#ifndef LLIST_H

typedef struct LListStruct{
    int length;
    int bufferLength;
    void * * data;
    
    void * (*add)(struct LListStruct *, void *, int);
    void * (*get)(struct LListStruct *, int);
    void   (*del)(struct LListStruct *);
    
} LList;


LList newLList();


#endif