#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LThread.h"



void * add5Point1AndReturnFloat(void * arg)
{
	int input = (int) arg;
	float output = (float)input + 5.1f;
	return *(void**) &output;
}

int main()
{
	//create a couple of threads
	LThread t1 = newLThread();
	LThread t2 = newLThread();

	//start t1 as add5Point1AndReturnFloat(), passing in 5
	t1.start(&t1, add5Point1AndReturnFloat, (void*)5);

	//sleep for 1 second
	lsleep(1000);

	//start t2 as add5Point1AndReturnFloat(), passing 100
	t2.start(&t2, add5Point1AndReturnFloat, (void*)100);

	
	void * r2 = t2.join(&t2); //wait for t2
	void * r1 = t1.join(&t1); //wait for t1

	float t1v = *(float*) &r1;
	float t2v = *(float*) &r2;
	
	//should print 
	printf("thread 1 returned %.2f, thread 2 returned %.2f\n", t1v, t2v);


	return 0;
}