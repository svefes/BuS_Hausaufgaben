#include <stdio.h>

int main(void)
{
	char zeile[] = "E  war einmal in einem Land vor unserer Zeit"; // 44 Buchstaben
    char* z;
    int i = 0;
    int* pi;

	zeile[1] = 's';
	
	printf("Hier kommt die a): %s\n", zeile);
	
	z = &zeile[43]-42;

	int *a = &i;
	
	printf("Nun kommt das s: %c\n", *z);

	return 0;
}
