#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"



int main(){
    
    //using lists:
    LList l = newLList(); //init
    
    LStr record = newLStr();
    
    int i;
    for(i = 0; i < 10; i++){
        l.add(&l, &i, sizeof(int)); //adding
    }
    
    
    for(i = 0; i < l.length; i++){ //getting
        int * ptr = l.get(&l, i);
        char buf[300];
        sprintf(buf, "l[%i] = %i\n", i, *ptr);
        record.addcs(&record, buf);
    }
    
    l.del(&l); //free all the data
    
    record.print(&record);
    //record.del(&record);
    
    
    
   return 0;
}