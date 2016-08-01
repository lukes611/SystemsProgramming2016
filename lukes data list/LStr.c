#include "LStr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void __del(LStr * self){
    
}

static void __init(LStr * self){
    self->del = __del;
}

static LStr __newLStr(int bl){
    LStr ret;
    ret.bufferLength = bl;
    ret.length = 0;
    ret.ptr = (char *) malloc(ret.bufferLength);
    __init(&ret);
    return ret;
}

LStr newLStr(){
    LStr ret = __newLStr(20);
    __init(&ret);
    return ret;
}

