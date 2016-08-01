#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"

struct Lexicon{
    char type[40]; //num, op, name, print, ;
    char value[40];
};

LList getLexes(char * fn){
    LList ret = newLList();
    FILE * fi = fopen("script1", "r");
    char buffer[2000];
    int nr = fread(buffer, 1, 2000, fi);
    buffer[nr] = 0x00;
    fclose(fi);
    
    
    
    printf("%s\n", buffer);
    return ret;
}

/*

lang:

c = 20
x = 5 * 2 + c
print x 


variable names, numbers, algebra

*/

int main(){
    
    //getLexes("script1");
    LStr string = newLStrc("1 + 2 * 6");
    
    puts(string.ptr);
    
    
   return 0;
}