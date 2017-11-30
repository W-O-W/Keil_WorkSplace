#include <stdio.h>
void main(){
	int (*p)[2];
	p = malloc(sizeof(int)*2);
	p=1;
	p+=sizeof(int);
	p=2;
    printf("%d",**p);
}
