#include "List.h"

static void __add__(List * self, int newInteger){
    //2 . if we need to: extend the size of our buffer
    if(self->length >= self->bufferLength){ //extend the size of buffer, and copy the values accross
        self->bufferLength = self->bufferLength * 2 + 1;
        self->data = realloc(self->data, self->bufferLength * sizeof(int));
    }
    //1. store newInteger into our list
    self->data[self->length] = newInteger;
    self->length ++;
}


void listInit(List * self){
    self->length = 0;
    self->bufferLength = 20;
    self->data = ( int * ) malloc(sizeof(int) * 20);
    self->add = __add__;
}

List newList(){
    List self;
    self.length = 0;
    self.bufferLength = 20;
    self.data = ( int * ) malloc(sizeof(int) * 20);
    self.add = __add__;
    return self;
}


void listAdd(List * self, int newInteger){
    //2 . if we need to: extend the size of our buffer
    if(self->length >= self->bufferLength){ //extend the size of buffer, and copy the values accross
        self->bufferLength = self->bufferLength * 2 + 1;
        //allocate more memory
        //int * newBuffer = malloc(sizeof(int) * self->bufferLength);
        //copy over the data
        //int i;
        //for(i = 0; i < self->length; i++) newBuffer[i] = self->data[i];
        //free the old data and set to the new one
        //free(self->data);
        //self->data = newBuffer;
        self->data = realloc(self->data, self->bufferLength * sizeof(int));
    }
    //1. store newInteger into our list
    self->data[self->length] = newInteger;
    self->length ++;
}

int * listGet(List * self, int index){
    return /*&self->data[index] same as: */ self->data + index;
}

void listFree(List * self){
    free(self->data);
    self->length = self->bufferLength = 0;
}