#include "LThread.h"

#ifdef WIN32

#include <Windows.h>
#include <process.h>
#include <stdio.h>

void lsleep(unsigned int millisecs)
{
	Sleep((DWORD)millisecs);
}

static unsigned int WINAPI threadF(void * dataIn)
{
	unsigned char * allData = (unsigned char *)dataIn;
	LThreadFunction * fptr = (LThreadFunction*) allData;
	void * * dptr = (void * *) (allData + sizeof(LThreadFunction));
	
	LThreadFunction f = *fptr;
	void * data = *dptr;

	void * returnValue = f(data);

	free(allData);

	return (unsigned int) returnValue;
}


static void detachedThread(LThread * self, void * (*f) (void *), void * data)
{
	unsigned char * allData = (unsigned char *)malloc(sizeof(f) + sizeof(data));
	LThreadFunction * fptr = (LThreadFunction*) allData;
	void * * dptr = (void * *) (allData + sizeof(f));
	
	*fptr = f;
	*dptr = data;


	self->id = (HANDLE) _beginthreadex(NULL, 0, threadF, allData, 0, NULL);
	if(self->id != NULL) CloseHandle(self->id);
}

static void startThread(LThread * self, void * (*f) (void *), void * data)
{
	unsigned char * allData = (unsigned char *)malloc(sizeof(f) + sizeof(data));
	LThreadFunction * fptr = (LThreadFunction*) allData;
	void * * dptr = (void * *) (allData + sizeof(f));
	
	*fptr = f;
	*dptr = data;


	self->id = (HANDLE) _beginthreadex(NULL, 0, threadF, allData, 0, NULL);
}

static void * joinThread(LThread * self)
{
	WaitForSingleObject(self->id, INFINITE);
	DWORD ret;
	GetExitCodeThread(self->id, &ret);
	return (void*) ret;
}

//mutex stuff

static LMutex __initMT()
{
	LMutex ret;
	if(!InitializeCriticalSectionAndSpinCount(&ret.id, 0x00000400))
	{
		puts("error creating mutex");
		exit(5);
	}
	return ret;
}

static LCMutex __initCM()
{
	LCMutex ret;
	InitializeConditionVariable(&ret.id);
	return ret;
}


static void __signalCM(LCMutex * self)
{
	WakeConditionVariable(&self->id);
}


static void __waitCM(LCMutex * self)
{
	SleepConditionVariableCS(&self->id, &self->mutex.id, INFINITE);
}


static void __broadcastCM(LCMutex * self)
{
	WakeAllConditionVariable(&self->id);
}

static void __freeMT(LMutex * self)
{
	DeleteCriticalSection(&self->id);
}


static void __lockMT(LMutex * self)
{
	EnterCriticalSection(&self->id);
}


static void __unlockMT(LMutex * self)
{
	LeaveCriticalSection(&self->id);
}

static void __freeCM(LCMutex * self)
{
	self->mutex.free(&self->mutex);
}

#else //linux

#include <pthread.h>
#include <unistd.h>

void lsleep(unsigned int millisecs)
{
	millisecs *= 1000;
	usleep((useconds_t)millisecs);
}

static void detachedThread(LThread * self, void * (*f) (void *), void * data)
{
	pthread_create(&self->id, NULL, f, data);
	pthread_detach(self->id);
}

static void startThread(LThread * self, void * (*f) (void *), void * data)
{
	pthread_create(&self->id, NULL, f, data);
}


static void * joinThread(LThread * self)
{
	void * ret;
	pthread_join(self->id, &ret);
	return ret;
}


//mutex stuff

static LMutex __initMT()
{
	LMutex ret;
	pthread_mutex_init(&ret.id, NULL);
	return ret;
}

static void __freeMT(LMutex * self)
{
	pthread_mutex_destroy(&self->id);
}


static void __lockMT(LMutex * self)
{
	pthread_mutex_lock(&self->id);
}


static void __unlockMT(LMutex * self)
{
	pthread_mutex_unlock(&self->id);
}

static LCMutex __initCM()
{
	LCMutex ret;
	pthread_cond_init(&ret.id, NULL);
	return ret;
}

static void __freeCM(LCMutex * self)
{
	self->mutex.free(&self->mutex);
	pthread_cond_destroy(&self->id);
}

static void __signalCM(LCMutex * self)
{
	pthread_cond_signal(&self->id);
}


static void __waitCM(LCMutex * self)
{
	pthread_cond_wait(&self->id, &self->mutex.id);
}


static void __broadcastCM(LCMutex * self)
{
	pthread_cond_broadcast(&self->id);
}


#endif

static void __lockCM(LCMutex * self)
{
	self->mutex.lock(&self->mutex);
}

static void __unlockCM(LCMutex * self)
{
	self->mutex.unlock(&self->mutex);
}


LThread newLThread(){
	LThread ret;

	ret.startDetached = detachedThread;
	ret.start = startThread;
	ret.join = joinThread;
	return ret;
}

LMutex newLMutex()
{
	LMutex ret = __initMT();
	ret.free = __freeMT;
	ret.lock = __lockMT;
	ret.unlock = __unlockMT;
	return ret;
}

LCMutex newLCMutex()
{
	LCMutex ret = __initCM();

	ret.mutex = newLMutex();

	ret.free = __freeCM;
	ret.lock = __lockCM;
	ret.unlock = __unlockCM;
	ret.signal = __signalCM;
	ret.wait = __waitCM;
	ret.broadcast = __broadcastCM;
	return ret;
}



