#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char *argv[])
{
	FILE *f;
	f = fopen(argv[1],"r");
	int i;
	while (fscanf(f,"%d",&i)==1) printf("%d ",2*i);
	printf("\n");
}
