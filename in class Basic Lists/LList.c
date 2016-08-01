#include "LList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void * __add(LList * self, void * data, int dataSize){
    if(self->length >= self->bufferLength){
        self->bufferLength *= 2;
        self->data = (void * * ) realloc((void*)self->data, self->bufferLength * sizeof(void *));
    }
    void * newData = malloc(dataSize);
    memcpy(newData, data, dataSize);
    self->data[self->length++] = newData;
    return newData;
}

void * __get(LList * self, int index){
    return self->data[index];
}

void __del(LList * self){}

LList newLList(){
    LList self;
    self.bufferLength = 10;
    self.length = 0;
    self.data = (void * * ) malloc(sizeof(void *) * self.bufferLength);
    
    //add functionality
    self.add = __add;
    self.get = __get;
    self.del = __del;
    
    return self;
}

int len(LList * self){ return self->length; }