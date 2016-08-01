#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"
#include "LexicalAnalysis.h"

/*

parser:

*/







int main(){
    
    LStr file = LA_file("script1");
    LList lexList = LA_lexicalAnalysis(file);
    for(int i = 0; i < lexList.length; i++){
        Lexicon * l = (Lexicon*)lexList.get(&lexList, i);
        int pb = l->type.eqcs(&l->type, "num") || l->type.eqcs(&l->type, "var") || l->type.eqcs(&l->type, "str");
        
        if(pb) printf("%s, %s\n", l->type.ptr, l->val.ptr);
        else printf("%s\n", l->type.ptr);
    }
    
 
    
    return 0;
}