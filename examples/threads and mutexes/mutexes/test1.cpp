#include <stdio.h>
#include <stdlib.h>
#include "LThread.h"

void * f(void * d)
{
	LMutex * m = (LMutex *)d;

	puts("in child, waiting to access lock...");

	m->lock(m);

	puts("access aquired for child... child sleeping for 3 seconds");

	lsleep(3000); //sleep a bit before unlocking lock

	m->unlock(m);

	return (void*)75;
}

int main()
{
	
	LThread t = newLThread(); //create thread
	
	LMutex m = newLMutex(); //create mutex
	
	m.lock(&m); //lock it
	
	t.start(&t, f, &m); //start the thread, pass the mutex (note: mutex could also be a global variable)

	
	puts("parent sleeping 3 seconds");
	lsleep(3000); //sleep for 3 seconds


	m.unlock(&m);

	lsleep(100); //sleep to allow child to access lock before parent re-locks it

	puts("parent waiting for lock...");
	//synchornize because child then has the lock
	m.lock(&m);
	puts("access aquired for parent again...");
	m.unlock(&m);

	int x = (int)t.join(&t); //wait for child to give back value

	printf("child says: %i\n", x);

	return 0;
}