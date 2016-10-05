#ifndef LTHREAD_H
#define LTHREAD_H


//sleep for x amount of milliseconds x-platform
void lsleep(unsigned int millisecs);

#ifdef WIN32
#include <Windows.h>
typedef HANDLE LTHREAD_ID;

#else 

#include <pthread.h>
typedef pthread_t LTHREAD_ID;

#endif

//a thread function type
typedef void*(*LThreadFunction)(void *);


typedef struct LThread_
{
	LTHREAD_ID id;
	void (*threadDetached)(struct LThread_ * self, LThreadFunction f, void * data); //start thread detatched (don't call join)
	void (*start)(struct LThread_ * self, LThreadFunction f, void * data); //start regular thread (call join to get return value and clean up memory)
	void * (*join)(struct LThread_ * self); //wait for thread to finish and get the return value
} LThread;

LThread newLThread();

#endif

