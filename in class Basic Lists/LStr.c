#include "LStr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void __del(LStr * self){
    
}

void __init(LStr * self){
    self->del = __del;
}

LStr newLStrn(int bl){
    LStr ret;
    ret.bufferLength = bl;
    ret.length = 0;
    ret.ptr = (char *) malloc(ret.bufferLength);
    __init(&ret);
    return ret;
}

LStr newLStr(){
    LStr ret = newLStrn(20);
    __init(&ret);
    return ret;
}

LStr newLStrc(char * input){
    int _size = strlen(input); 
    LStr ret = newLStrn(_size+2);
    strcpy(ret.ptr, input);
    __init(&ret);
    return ret;
}
