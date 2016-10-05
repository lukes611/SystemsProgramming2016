#ifndef LTHREAD_H
#define LTHREAD_H


//sleep for x amount of milliseconds
void lsleep(unsigned int millisecs);

#ifdef WIN32
#include <Windows.h>
typedef HANDLE LTHREAD_ID;
typedef CRITICAL_SECTION LTHREAD_MUTEX_ID;

#else //linux

#include <pthread.h>
typedef pthread_t LTHREAD_ID;
typedef pthread_mutex_t LTHREAD_MUTEX_ID;
#endif

typedef void*(*LThreadFunction)(void *);


typedef struct LThread_
{
	LTHREAD_ID id;
	void (*threadDetached)(struct LThread_ * self, LThreadFunction f, void * data);
	void (*start)(struct LThread_ * self, LThreadFunction f, void * data);
	void * (*join)(struct LThread_ * self);
} LThread;

LThread newLThread();

typedef struct LMutex_
{
	LTHREAD_MUTEX_ID id;
	void (*free) (struct LMutex_ * self);
	void (*lock) (struct LMutex_ * self);
	void (*unlock) (struct LMutex_ * self);
} LMutex;



LMutex newLMutex();

#endif

