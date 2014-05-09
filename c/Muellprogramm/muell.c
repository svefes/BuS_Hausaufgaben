#include <stdio.h>

int main(void){
	char *test;
	int i = 0;

	test = 0;
	*test=0;

	while(1){
		printf("Nummer %d:  %c\n",i, *test);
		i++;
		test = i;
	}
	
	return 0;	
}
