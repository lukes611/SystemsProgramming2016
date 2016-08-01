#include "LList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



static void * __add(LList * self, void * data, int dataSize){
    if(self->length >= self->bufferLength){
        self->bufferLength = self->bufferLength * 2 + 1;
        self->data = (void * * ) realloc((void*)self->data, self->bufferLength * sizeof(void *));
    }
    void * newData = malloc(dataSize);
    memcpy(newData, data, dataSize);
    self->data[self->length++] = newData;
    return newData;
}

static void * __get(LList * self, int index){
    return self->data[index];
}

static void __del(LList * self){
    int i = 0;
    for(; i < self->length; i++) free(self->get(self, i));
    free(self->data);
    self->length = 0;
    self->bufferLength = 0;
}

static void __swap(LList * self, int index1, int index2){
    void * temp = self->data[index1];
    self->data[index1] = self->data[index2];
    self->data[index2] = temp;
}

static int __partition(LList * self, int from, int to, int (*f) (void *, void *)){
    int j = from;
    for(int i = from; i < to; i++){
        if(f(self->data[i], self->data[to]) <= 0){
            __swap(self, i, j);
            j++;
        }
    }
    __swap(self, j, to);
    return j;
}


static void __qs(LList * self, int from, int to, int (*f) (void *, void *)){
    if(from < to){
        int p = __partition(self, from, to, f);
        __qs(self, from, p-1, f);
        __qs(self, p, to, f);
    }
}

static void __sort(LList * self, int (*f)(void *, void *)){
    __qs(self, 0, self->length-1, f);
}

static void __set(LList * self, int index, void * newData, int newDataSize){
    free(self->data[index]);
    void * nd = malloc(newDataSize);
    memcpy(nd, newData, newDataSize);
    self->data[index] = nd;
}

static void __remove(LList * self, int index){
    int i = 0;
    free(self->data[index]);
    for(i = index; i < self->length - 1; i++){
        self->data[i] = self->data[i+1];
    }
    self->length -= 1;
}



LList newLList(){
    LList self;
    self.bufferLength = 10;
    self.length = 0;
    self.data = (void * * ) malloc(sizeof(void *) * self.bufferLength);
    
    //add functionality
    self.add = __add;
    self.get = __get;
    self.del = __del;
    self.sort = __sort;
    self.remove = __remove;
    self.set = __set;
    return self;
}

