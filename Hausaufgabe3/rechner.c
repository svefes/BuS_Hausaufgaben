#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

struct summanden_ {
	int a;
	int b;
};

int main (void) {
	int vomrechner[2], zumrechner[2];
	int pid;
	char buf[20];
	struct summanden_ summanden;
	int summe;
	//int nbytes;
	

	if ( (pipe(vomrechner) != 0) || (pipe(zumrechner) != 0)) {
		printf("pipe(): %d %s\n", errno, strerror(errno));
		exit(1);
	}

	pid = fork();
	if (pid == -1) {
		printf("fork(): %d %s\n", errno, strerror(errno));
		exit(1);
	}
	else if (pid == 0) { // Kind
		close(vomrechner[0]);
		close(zumrechner[1]);

		/* Hier Code */
		int status;
		status = 1;
		
		while(status == 1)
		{
			printf("Kind: Ich warte auf Vater.\n");
			if(read(zumrechner[0], &summanden, sizeof(struct summanden_)) < 0)
			{
				printf("read(): %d %s\n", errno, strerror(errno));
				exit(1);
			}
			printf("\nKind: Ich habe erhalten: %d %d\n", summanden.a, summanden.b);
		
			if(summanden.a == 0 && summanden.b == 0)
			{
				status = 0;
				printf("Kind: Abbruch einleiten!\n");
			}
			else{
		
				summe = summanden.a + summanden.b;
			
				if(write(vomrechner[1], &summe, sizeof(int)) < 0)
				{
					printf("write(): %d %s\n", errno, strerror(errno));
					exit(1);
				}
			}
		}
		
		close(vomrechner[1]);
		close(zumrechner[0]);
		printf("Kind: fertig!\n");
		
		
	}
	else { // Vater
		close(vomrechner[1]);
		close(zumrechner[0]);

		/* Hier Code */
		int status;
		
		status = 1;
		
		while(status == 1)
		{
			printf("Erste Zahl: \n");
			fgets(buf, 20, stdin);
			summanden.a = atoi(buf);
		
			printf("Zweite Zahl: \n");
			fgets(buf, 20, stdin);
			summanden.b = atoi(buf);
			
			if(write(zumrechner[1], &summanden, sizeof(struct summanden_)) < 0)
			{
				printf("write(): %d %s\n", errno, strerror(errno));
				exit(1);
			}
		
			if(summanden.a == 0 && summanden.b == 0)
			{
				status = 0;
				printf("Vater: Abbruch einleiten!\n");
			}
			else
			{
				printf("Vater: Ihre Eingaben: %d %d\n", summanden.a, summanden.b);
		
				if(read(vomrechner[0], &summe, sizeof(int)) < 0)
				{
					printf("read(): %d %s\n", errno, strerror(errno));
					exit(1);
				}
		
				printf("Vater: Das Ergebnis ist %d\n\n", summe);
			}
		}
		
		close(vomrechner[0]);
		close(zumrechner[1]);
		waitpid(pid, NULL, 0);
		printf("Vater: fertig!\n");

	}

	return 0;
}
