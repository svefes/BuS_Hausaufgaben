#include <stdio.h>

int main(int argc, char **argv)
{
	int i;	

	//printf("\n\n\n");
	printf("Anzahl Argumente: %d\n", argc); //Ausgabe der Anzahl der Argumente
	
	//Ausgabe aller Argumente
	for(i = 1; i < argc; i++)
	{
		printf("Text: %s\n", argv[i]);
	}
	return 0;
}
