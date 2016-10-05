#include "LThread.h"

#ifdef WIN32

#include <Windows.h>
#include <process.h>
#include <stdio.h>

void lsleep(unsigned int millisecs) 
{
	Sleep((DWORD)millisecs);
}

static unsigned int WINAPI threadF(void * dataIn) //a thread function which can be used by windows (used to emulate pthread's version)
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


static void detachedThread(LThread * self, LThreadFunction f, void * data)
{
	unsigned char * allData = (unsigned char *)malloc(sizeof(f) + sizeof(data));
	LThreadFunction * fptr = (LThreadFunction*) allData;
	void * * dptr = (void * *) (allData + sizeof(f));
	
	*fptr = f;
	*dptr = data;


	self->id = (HANDLE) _beginthreadex(NULL, 0, threadF, allData, 0, NULL);
	if(self->id != NULL) CloseHandle(self->id);
}

static void startThread(LThread * self, LThreadFunction f, void * data)
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


#endif
LThread newLThread(){
	LThread ret;

	ret.threadDetached = detachedThread;
	ret.start = startThread;
	ret.join = joinThread;

	return ret;
}



