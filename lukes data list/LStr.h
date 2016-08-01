#ifndef LSTR_H

typedef struct LStrStruct{
    int bufferLength;
    char * ptr;
    
    void (*del)(struct LStrStruct *);
    void (*addc)(struct LStrStruct *, char);
    void (*addcs)(struct LStrStruct *, char *);
    void (*setcs)(struct LStrStruct *, char *);
    void (*add)(struct LStrStruct *, struct LStrStruct *);
    int  (*length)(struct LStrStruct *);
    void (*print)(struct LStrStruct *);
    
    void (*addi)(struct LStrStruct *, int);
    void (*addf)(struct LStrStruct *, float);
    void (*addd)(struct LStrStruct *, double);
    
    int (*parseInt)(struct LStrStruct *);
    float (*parseFloat)(struct LStrStruct *);
    double (*parseDouble)(struct LStrStruct *);
    
}LStr;

LStr newLStr();


#endif