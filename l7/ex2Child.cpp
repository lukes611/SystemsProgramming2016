#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prefix.h"

#include <io.h>

int main(int N, char * * strings)
{
	
	int reader = _fileno(stdin);
	int writer = _fileno(stdout);

	FILE * log = fopen("C:/Users/luke/Desktop/ChildLog.txt", "a");
	int i;
	for(i = 0; i < N; i++) fprintf(log, "%i. %s\n", i, strings[i]);
	fclose(log);
	while(1)
	{
		char buf[101];
		int nr = _read(reader, buf, 100);
		buf[nr] = 0;

		if(strcmp("quit", buf) == 0) break;
		sprintf(buf, "ans = %lf", prefixCalc(buf));
		_write(writer, buf, strlen(buf));
	}
	
	
}

