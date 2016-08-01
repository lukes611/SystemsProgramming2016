#include "LStr.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void __del(LStr * self){
    free(self->ptr);
    self->bufferLength = 0;
}

static int __getLength(LStr * self){
    return strlen(self->ptr);
}

static void __setcs(LStr * self, char * c_str){
    int csl = strlen(c_str);
    if(csl > self->bufferLength){
        self->bufferLength = csl;
        self->ptr = (char *) realloc(self->ptr, self->bufferLength);
    }
    strcpy(self->ptr, c_str);
}

static void __print(LStr * self){
    puts(self->ptr);
}

static void __add(LStr * self, LStr * other){
    int newSize = other->length(other) + self->length(self);
    if(newSize >= self->bufferLength){
        self->bufferLength = newSize + 10;
        self->ptr = (char *) realloc(self->ptr, self->bufferLength);
    }
    strcat(self->ptr, other->ptr);
}

static void __addc(LStr * self, char other){
    char cArr[2]; cArr[0] = other; cArr[1] = 0x00;
    LStr tmp = newLStr();
    tmp.setcs(&tmp, cArr);
    self->add(self, &tmp);
    tmp.del(&tmp);
}

static void __addcs(LStr * self, char * other){
    LStr tmp = newLStr();
    tmp.setcs(&tmp, other);
    self->add(self, &tmp);
    tmp.del(&tmp);
}

static void __addi(LStr * self, int nv){
    char buffer[200];
    sprintf(buffer, "%i", nv);
    self->addcs(self, buffer);
}

static void __addf(LStr * self, float nv){
    char buffer[200];
    sprintf(buffer, "%f", nv);
    self->addcs(self, buffer);
}

static void __addd(LStr * self, double nv){
    char buffer[200];
    sprintf(buffer, "%lf", nv);
    self->addcs(self, buffer);
}

static int __parseInt(LStr * self){
    int ret = 0;
    sscanf(self->ptr, "%i", &ret);
    return ret;
}

static float __parseFloat(LStr * self){
    float ret = 0.0f;
    sscanf(self->ptr, "%f", &ret);
    return ret;
}

static double __parseDouble(LStr * self){
    double ret = 0.0;
    sscanf(self->ptr, "%lf", &ret);
    return ret;
}

static void __init(LStr * self){
    self->del = __del;
    self->length = __getLength;
    self->setcs = __setcs;
    self->print = __print;
    self->add = __add;
    self->addcs = __addcs;
    self->addc = __addc;
    self->addi = __addi;
    self->addf = __addf;
    self->addd = __addd;
    self->parseInt = __parseInt;
    self->parseDouble = __parseDouble;
    self->parseFloat = __parseFloat;
}

static LStr __newLStr(int bl){
    LStr ret;
    ret.bufferLength = bl;
    ret.ptr = (char *) malloc(ret.bufferLength);
    ret.ptr[0] = 0x00;
    __init(&ret);
    return ret;
}

LStr newLStr(){
    LStr ret = __newLStr(20);
    __init(&ret);
    return ret;
}

