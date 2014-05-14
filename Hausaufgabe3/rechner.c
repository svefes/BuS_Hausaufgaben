#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
	int nbytes;

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

	}
	else { // Vater
		close(vomrechner[1]);
		close(zumrechner[0]);

		/* Hier Code */

	}

	return 0;
}
