#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"



int main(){
    
    //using lists:
    LList l = newLList();
    int i;
    for(i = 0; i < 10; i++){
        l.add(&l, &i, sizeof(int)); //adding
    }
    
    
    for(i = 0; i < l.length; i++){ //getting
        int * ptr = l.get(&l, i);
        printf("l[%i] = %i\n", i, *ptr);
    }
    
    l.del(&l); //free all the data
    
    
    
    
   return 0;
}