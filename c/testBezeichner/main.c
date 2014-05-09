#include <stdio.h>
void main(){
	int *test;
	printf("%p\n", test);
	
	int a = 5;
	test = &a;
	
	printf("%p\n", test);
}
