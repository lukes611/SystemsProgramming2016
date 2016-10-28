

#include "SMem.h"
#include <unistd.h>

int main(){
	puts("Client Started");

	int id = 8142;
	int amountOfMemory = sizeof(int) * 2;

	//create shared memory
	SharedMemory sm = newSharedMemory(id, amountOfMemory, SMEM_ACCESS);

	if(sm.error){
		puts("error with shared memory");
		return 1;
	}
	
	int * sync = (int*)sm.ptr;
	int * number = sync + 1;


	int num;
	printf("enter a number: "), scanf("%d", &num); //set num

	while(*sync != 0) ;// wait for sync to be 0

	*number = num;
	*sync = 1;

	while(*sync != -1){
		if(*sync == 2){
			int fCopy = *number;
			*sync = 0;
			printf("factor found: %d\n", *number);
		}
	}	


	puts("client shutdown");

	return 0;
}


