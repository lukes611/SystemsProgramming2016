#ifndef LTHREAD_H
#define LTHREAD_H


//sleep for x amount of milliseconds
void lsleep(unsigned int millisecs);

#ifdef WIN32
#include <Windows.h>
typedef HANDLE LTHREAD_ID;

#else 

#include <pthread.h>
typedef pthread_t LTHREAD_ID;

#endif

typedef void*(*LThreadFunction)(void *);


typedef struct LThread_
{
	LTHREAD_ID id;
	void (*startDetached)(struct LThread_ * self, LThreadFunction f, void * data);
	void (*start)(struct LThread_ * self, LThreadFunction f, void * data);
	void * (*join)(struct LThread_ * self);
	
} LThread;

LThread newLThread();

typedef struct LMutex_
{
#ifdef WIN32 //windows
	CRITICAL_SECTION id;
#else //linux
	pthread_mutex_t id;

#endif //x-platform

	void (*free) (struct LMutex_ * self);
	void (*lock) (struct LMutex_ * self);
	void (*unlock) (struct LMutex_ * self);

} LMutex;



LMutex newLMutex();

typedef struct LCMutex_
{
#ifdef WIN32 //windows
	CONDITION_VARIABLE id;
#else //linux
	pthread_cond_t id;

#endif //x-platform

	LMutex mutex;

	void (*free) (struct LCMutex_ * self);
	void (*lock) (struct LCMutex_ * self);
	void (*unlock) (struct LCMutex_ * self);
	void (*wait) (struct LCMutex_ * self);
	void (*signal) (struct LCMutex_ * self);
	void (*broadcast) (struct LCMutex_ * self);

} LCMutex;

LCMutex newLCMutex();

#endif

