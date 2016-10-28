

#include "SMem.h"
#include <unistd.h>

int main(){
	puts("Client Started");

	int id = 8142;
	int amountOfMemory = sizeof(int) * 2;

	//create shared memory
	SharedMemory sm = newSharedMemory(id, amountOfMemory, SMEM_ACCESS);

	//quit if error
	if(sm.error){
		puts("error with shared memory");
		return 1;
	}
	
	//set up pointers to data
	int * sync = (int*)sm.ptr;
	int * number = sync + 1;

	//ask the user to enter a number to send to the server to factor
	int num;
	printf("enter a number: "), scanf("%d", &num); //set num

	while(*sync != 0) /*wait while the server is not read*/;

	*number = num; //write the number to shared memory
	*sync = 1; //signal the server the number is available

	while(*sync != -1){ //whilst the server is not finished
		if(*sync == 2){ //if there is a factor
			int fCopy = *number; //read it in from shared memory
			*sync = 0; //signal that we got that factor to the server
			printf("factor found: %d\n", *number);
		}
	}	


	puts("client shutdown");

	return 0;
}


