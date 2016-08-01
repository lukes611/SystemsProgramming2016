#ifndef LLIST_H
#define LLIST_H

typedef struct LListStruct{
    int length;
    int bufferLength;
    void * * data;
    
    void * (*add)(struct LListStruct *, void *, int);
    void * (*get)(struct LListStruct *, int);
    void (*set)(struct LListStruct *, int, void *, int);
    void   (*del)(struct LListStruct *);
    void (*sort)(struct LListStruct *, int (*)(void *, void *));
    void (*remove)(struct LListStruct *, int index);
    
} LList;


LList newLList();


#endif