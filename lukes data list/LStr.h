#ifndef LSTR_H

typedef struct LStrStruct{
    int length, bufferLength;
    char * ptr;
    
    void (*del)(struct LStrStruct *);
    
}LStr;

LStr newLStr();


#endif