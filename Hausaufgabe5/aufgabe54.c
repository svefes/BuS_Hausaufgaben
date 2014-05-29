#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAX_CUSTOMERS 8
#define MAX_QUEUE_LENGTH 10
#define MAX_PROCESSES (MAX_CUSTOMERS + MAX_QUEUE_LENGTH)

struct nightclub_s {
    pid_t pid[MAX_PROCESSES]; /* enthaelt die PIDs der Kindprozesse */
    sem_t free_space_inside;  /* das ist der Türsteher, er darf nur MAX_CUSTOMERS rein lassen */
    int customers_in_queue;  /* die Anzahl an Leuten die vor der Tür warten, darf nicht größer als MAX_QUEUE_LENGTH werden */
};


int shouldEnd = 0;   /* to terminate the loop */

/* SIGINT signal handler */
void signal_handler(int signalNum) {
    printf("Parent: Got interrupted, will shutdown now\n");
    shouldEnd = 1;
}


int main(int argc, char **argv)
{
    int id, shm_sem_id,  i;   /* "id" fuer das Shared Memory Segment
                  mit "*shar_mem" kann der im Shared Memory
                  gespeicherte Wert veraenderts werden     */
    struct nightclub_s* shar_mem;
    pid_t f_pid;    /* the pid after fork */
   	sem_t* shm_sem;
    
    /* Nun wird das Shared Memory Segment angefordert, an den Prozess
     angehaengt, und auf 0 gesetzt */
    id = shmget(IPC_PRIVATE, sizeof(struct nightclub_s), IPC_CREAT|0644);
    shar_mem = (struct nightclub_s*)shmat(id, 0, 0);
    memset(shar_mem, 0, sizeof(struct nightclub_s));
    
    /* initialisiere pids mit -1 */
    for(i=0; i < MAX_PROCESSES; i++) {
        shar_mem->pid[i] = -1;
    }
    
    
    /* shar_mem->free_space_inside ist eine Zählsemaphore die angibt, wieviel Platz im Club ist
     *
     * Am Anfang kann sie also MAX_CUSTOMERS viele Leute reinlassen
     * Was wäre also ein guter start Wert? Wann blockt die Semaphore?
     *
     * Achtung: shar_mem->free_space_inside ist die Semaphore um zu Zählen wieviele Leute im Club sind,
     * sie sichert nicht den gemeinsamen Speicherbereich (shar_mem), dazu benötigen Sie eine 2. unbenannte
     * Semaphore, die sie noch anlegen müssen.
     */
     
	shm_sem_id = shmget(IPC_PRIVATE, sizeof(sem_t), IPC_CREAT|0644);
	shm_sem = (sem_t*)shmat(shm_sem_id, 0, 0);
	memset(shm_sem, 0, sizeof(sem_t));
	
	sem_init(&shar_mem->free_space_inside, 1, MAX_CUSTOMERS);
	sem_init(shm_sem, 1, 1);
     
    
    /* initialize random number generator */
    srand(time(NULL));
    
    /* catch interrupts */
    signal(SIGINT, signal_handler);
    
    while (!shouldEnd) {
        /* ist noch platz in der Schlange? */
        if(shar_mem->customers_in_queue < MAX_QUEUE_LENGTH) {
            /* there is space for one more */
            for(i = 0; i < MAX_PROCESSES; i++) {
                if(shar_mem->pid[i] == -1) {
                    break;
                }
            }
            
            /* enque customer in line */
            sem_wait(shm_sem);
            shar_mem->customers_in_queue++;
            sem_post(shm_sem);
            
            /* create the new customer */
            f_pid = fork();
            if (f_pid == 0) {
                /* Das passiert nur im Kind */
                struct timespec tv;
                
                /* Kinder sollen nicht auf SIG_INT hören */
                signal(SIGINT, SIG_DFL);
                
                /* Andere Zufallszahlen als der Vater bitte! */
                srand(time(NULL));
                
                tv.tv_sec = time(NULL) + 2;
                
                
                //TODO Gucken Zugriff auf shared memory
                if(sem_timedwait(&shar_mem->free_space_inside, &tv) != 0)
                {
                    printf("%d: That takes too long, I leave\n", getpid());
                    
                    /* Lösche unsere PID, denn wir beenden uns jetzt */
                    for(i = 0; i < MAX_PROCESSES; i++) {
                        if(shar_mem->pid[i] == getpid()) {
                        	sem_wait(shm_sem);
                            shar_mem->pid[i] = -1;
                            sem_post(shm_sem);
                            break;
                        }
                    }
                    /* Und aus der Queue raus! */
                    sem_wait(shm_sem);
                    shar_mem->customers_in_queue--;
                    sem_post(shm_sem);
                }else {
                    /* Wir sind drinn, also Warteschlange verlassen */
                    sem_wait(shm_sem);
                    shar_mem->customers_in_queue--;
                    sem_post(shm_sem);
                    
                    /* Jetzt ist hier Party angesagt */
                    printf("%d: PARTY PARTY\n", getpid());
                    usleep(((rand() % 5000) + 3000) * 1000);
                    printf("%d: I go home now\n", getpid());
                    
                    /* Lösche unsere PID, denn wir beenden uns jetzt */
                    for(i = 0; i < MAX_PROCESSES; i++) {
                        if(shar_mem->pid[i] == getpid()) {
                            sem_wait(shm_sem);
                            shar_mem->pid[i] = -1;
                            sem_post(shm_sem);
                            break;
                        }
                    }
                    
                    sem_post(&shar_mem->free_space_inside);
                }
                /* und jetzt beenden wir das Kind */
                exit(0);
            }else {
                /* Das ist der Vater, gib also aus wer in die Queue gekommen ist und wieviele Leute drin sind */
                sem_wait(shm_sem);
                shar_mem->pid[i] =  f_pid;
                sem_post(shm_sem);
                printf("Parent: %d joined the queue, there are %d people in the queue\n",
                       f_pid, shar_mem->customers_in_queue);
            }
        }
        
        /* Das ganze etwas verlangsamen */
        usleep(((rand()% 501)+300)*1000);
    }
    
    /* ok we should end here so wait for all children to terminate */
    printf("Parent: Lights on, wait for customers to leave\n");
    
    for(i = 0; i < MAX_PROCESSES; i++) {
        if (shar_mem->pid[i] != -1) {
            waitpid(shar_mem->pid[i], NULL, 0);
        }
    }
    
    /* Die Shared Memory Segmente werden abgekoppelt und freigegeben. */
    shmdt(shar_mem);
    shmctl(id, IPC_RMID, 0);
    
    /* Und auch noch für die semaphore die den shared mem beschützt */
    shmdt(shm_sem);
    shmctl(shm_sem_id, IPC_RMID, 0);
    
    return 0;
}
