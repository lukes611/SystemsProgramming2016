#ifndef LSTR_H
#define LSTR_H

#include "LList.h"

typedef struct LStrStruct{
    int bufferLength;
    char * ptr;
    
    void (*del)(struct LStrStruct *);
    void (*addc)(struct LStrStruct *, char);
    void (*addcs)(struct LStrStruct *, char *);
    void (*setcs)(struct LStrStruct *, char *);
    void (*set)(struct LStrStruct *, struct LStrStruct *);
    void (*add)(struct LStrStruct *, struct LStrStruct *);
    int  (*length)(struct LStrStruct *);
    void (*print)(struct LStrStruct *);
    
    int (*findc)(struct LStrStruct *, char);
    
    void (*addi)(struct LStrStruct *, int);
    void (*addf)(struct LStrStruct *, float);
    void (*addd)(struct LStrStruct *, double);
    
    int (*eqcs)(struct LStrStruct *, char *);
    int (*eq)(struct LStrStruct *, struct LStrStruct *);
    
    int (*parseInt)(struct LStrStruct *);
    float (*parseFloat)(struct LStrStruct *);
    double (*parseDouble)(struct LStrStruct *);
    
    LList (*split)(struct LStrStruct *, char);
    
}LStr;

LStr newLStr();


#endif