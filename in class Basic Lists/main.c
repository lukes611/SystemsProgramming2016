#include <stdio.h>
#include <stdlib.h>
#include "List.h"

//create my list object


int main(){
    
    
    //List myList;
    //listInit(&myList);
    List myList = newList();
    
    
    int i;
    for(i = 0; i < 25; i++)
        myList.add(&myList, i);
    
    printf("length: %i, bufferLength: %i\n", myList.length, myList.bufferLength);
    
    
    
    for(i = 0; i < myList.length; i++)
        printf("%i\n", *listGet(&myList, i));
    
    
    listFree(&myList);
    
    
    
    
    return 0;
}