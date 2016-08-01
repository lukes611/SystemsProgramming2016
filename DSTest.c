#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"

int compFunc(void * p1, void * p2){
    int * a = (int*)p1;
    int * b = (int*)p2;
    return *a - *b;
}


int main(){
    
    
    
    //using lists:
    LList l = newLList(); //init
    
    LStr record = newLStr();
    
    int i;
    for(i = 0; i < 5; i++){
        int v = rand() % 200;
        l.add(&l, &v, sizeof(int)); //adding
    }
    
    
    for(i = 0; i < l.length; i++){ //getting
        int * ptr = l.get(&l, i);
        char buf[300];
        sprintf(buf, "l[%i] = %i\n", i, *ptr);
        record.addcs(&record, buf);
    }
    
   
    
    record.print(&record);
    
    printf("************\n");
    
    l.sort(&l, compFunc);
    
    for(i = 0; i < l.length; i++) printf("l[%i] = %i\n", i, *((int*)l.get(&l, i)));
    int nv = 2016;
    l.set(&l, 0, &nv, sizeof(nv));
    
    l.sort(&l, compFunc);
    
    printf("removing index 0\n***************\n");
    while(l.length >= 1){
        int * ptr = l.get(&l, 0);
        printf("%i\n", *ptr);
        l.remove(&l, 0);
    }
    
    
    record.del(&record);
     //l.del(&l); //free all the data
    
   return 0;
}