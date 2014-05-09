#include <stdio.h>

int main(void)
{
	char a*;
	char b = 'c';
	a = &b;
	b = 'h';
	printf("%c\n", *a);
}
