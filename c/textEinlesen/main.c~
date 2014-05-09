#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int fh, count; 
	
	fh = open(argv[1], O_RDONLY);	
	 
	if(fh != -1)
	{
		char puffer;
		while((count = read(fh, &puffer, sizeof(puffer))))
		{
			printf("%c", puffer);
		}
		close(fh);
	}
	else
	{
		printf("%s\n", strerror(errno));
		//printf("Datei kann nicht gefunden werden.");
	}
	
	return 0;
}
