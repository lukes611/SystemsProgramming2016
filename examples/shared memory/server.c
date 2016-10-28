

#include "SMem.h"
#include <unistd.h>



int main(){
	puts("Server Started");

	int id = 8142;
	int amountOfMemory = sizeof(int) * 2;

	//create shared memory
	SharedMemory sm = newSharedMemory(id, amountOfMemory, SMEM_CREATE);

	
	if(sm.error){
		puts("error with shared memory");
		return 1;
	}
	
	int * sync = (int*)sm.ptr;
	int * number = sync + 1;

	*sync = 0;
	*number = -1;

	puts("ready...");

	while(*sync != 1) ; //wait for sync to be 1
	int numCopy = *number;

	printf("client says: %d\n", numCopy);

	*sync = 0;

	int i;
	for(i = 1; i <= numCopy; i++){
		if(numCopy%i==0)/*if factor*/{
			while(*sync == 2) ;
			*number = i;
			*sync = 2;
		}
	}

	while(*sync == 2) ;
	*sync = -1;

	puts("server shutdown");
	return 0;
}


