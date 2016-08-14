#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp(const void * _a, const void * _b)
{
	char * a = *(char * * )_a;
	char * b = *(char * * )_b;
	int A = atoi(a);
	int B = atoi(b);
	return A - B;
}

int main()
{
	char buf[100], *toks[100], *s;
	int i, j;
	printf("enter some numbers: ");
	
	gets(buf);
	s = strtok(buf, " ");
	for(i = 0; s; i++, s = strtok(NULL, " ")) toks[i] = s;

	qsort(toks, i, sizeof(char *), comp);

	for(j=0; j < i; j++)
		puts(toks[j]);
	
}