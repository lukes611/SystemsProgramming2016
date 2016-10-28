
#ifndef SMEM_H

#define SMEM_H


//include some standard libraries
#include <stdio.h>
#include <stdlib.h>

//include libraries needed for shared memory
#include <sys/ipc.h>
#include <sys/shm.h>

//a struct to hold some info about shared memory
typedef struct _SharedMemory{
	int id, handle, size, error;
	void * ptr;
}SharedMemory;

#define SMEM_CREATE 0
#define SMEM_ACCESS 1

//create a new shared memory
SharedMemory newSharedMemory(int id, int sizeInBytes, int create){
	//create shared memory struct
	SharedMemory sm;

	//set fields
	sm.id = id, sm.size = sizeInBytes, sm.error = 0, sm.ptr = NULL;

	//set flag based on whether we are creating or accessing shared memory
	int flag = create == SMEM_CREATE ? IPC_CREAT | 0644 : 0644;

	//create or access shared memory from OS
	sm.handle = shmget(sm.id, sm.size, flag);

	//handle error
	if(sm.handle == -1){
		sm.error = 1;
		return sm;
	}
	//set pointer
	sm.ptr = shmat(sm.handle, NULL, 0);

	return sm;
}
void freeSharedMemory(SharedMemory * handle){
	//free the data
	shmdt(handle->ptr);
}



#endif
