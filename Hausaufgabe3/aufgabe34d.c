#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
int zaehler;

void* thread_func()
{
	int wait_time;
	for(; zaehler <= 50; zaehler++)
	{
		printf("Thread: %d\n", zaehler);
		if(zaehler <= 25)
		{
			usleep(300 * 1000);
		}
		else
		{
			srand(time(NULL));
			wait_time = (rand() %401) + 100;
			usleep(wait_time * 1000);
		}
	}
	
	return 0;
}

int main(void)
{
	int wait_time;
	pthread_t tid;
	zaehler = 1;
	
	for(; zaehler <= 50; zaehler++)
	{
		printf("Main: %d\n", zaehler);
		if(zaehler == 9)
		{
			pthread_create(&tid, NULL, thread_func, NULL);
		}
	
		if(zaehler <= 25)
		{
			usleep(300 * 1000);
		}
		else
		{
			srand(time(NULL));
			wait_time = (rand() %401) + 100;
			usleep(wait_time * 1000);
		}
	}
	
	return 0;
	
}
