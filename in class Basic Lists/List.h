
#ifndef LIST_H

#define LIST_H

#include <stdio.h>
#include <stdlib.h>


typedef struct List_Struct{
    int length, bufferLength;
    int * data;
    
    
    //private namespace for add:
    void (*add) (struct List_Struct *, int);
} List;

void listInit(List * self);
List newList();
void listAdd(List * self, int newInteger);
int * listGet(List * self, int index);
void listFree(List * self);




#endif