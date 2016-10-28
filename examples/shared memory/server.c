

#include "SMem.h"
#include <unistd.h>



int main(){
	puts("Server Started");

	int id = 8142;
	int amountOfMemory = sizeof(int) * 2;

	//create shared memory
	SharedMemory sm = newSharedMemory(id, amountOfMemory, SMEM_CREATE);

	
	//quit if error
	if(sm.error){
		puts("error with shared memory");
		return 1;
	}
	
	//get some pointers into shared memory
	int * sync = (int*)sm.ptr;
	int * number = sync + 1;

	//initialize them
	*sync = 0;
	*number = -1;

	puts("ready...");

	while(*sync != 1) ; //wait for client to enter number
	int numCopy = *number; //get number

	printf("client says: %d\n", numCopy); //print number

	*sync = 0; //reset sync

	int i;
	for(i = 1; i <= numCopy; i++){ //for each number in range 1-N
		if(numCopy%i==0)/*if factor, give to client*/{
			while(*sync == 2) /*wait until client reads any old values*/;
			*number = i; //write number to shared memory
			*sync = 2; //let the server know there is a factor to read
		}
	}

	while(*sync == 2) /*wait until client reads any old values*/;
	*sync = -1; //signal client to quit

	puts("server shutdown");
	return 0;
}


