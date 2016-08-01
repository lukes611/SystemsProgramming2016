#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"
#include "LexicalAnalysis.h"

/*
lexical analyse:

what to have: basics, lists, objects

*/



LList lexicalAnalysis(LStr sf){
    LList ret = newLList();
    
    sf.addc(&sf, ';');
    int index = 0;
    int state = 0;
    LStr names = newLStr(); names.setcs(&names, "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP$");
    LStr numbers = newLStr(); numbers.setcs(&numbers, "0123456789.");
    LStr singles = newLStr(); singles.setcs(&singles, "+*-/=!()[].%{}");
    
    //doubles: != == <= >= += -= *= /= %=
    //singles ! - + * / [ ] ( ) . = % { }
    //key words: print not or and
    
    LStr temp = newLStr();
    
    int fileLen = sf.length(&sf);
    
    while(index < fileLen){
        
        char c = sf.ptr[index];
        char both[3] = {0x00, 0x00, 0x00};
        char cs[2] = {c, 0x00};
        int canHaveBoth = index + 1 < fileLen;
        if(canHaveBoth){
            both[0] = c;
            both[1] = sf.ptr[index+1];
        }
        LStr bothstring = newLStr();
        bothstring.setcs(&bothstring, both);
        
        
        if(state == 0){
            if(c == ';'){
                Lexicon nl = newLexicon(";", ";");
                ret.add(&ret, &nl, sizeof(Lexicon));
            }else if(names.findc(&names, c) == 1){
                temp.setcs(&temp, "");
                temp.addc(&temp, c);
                state = 1;
            }else if(canHaveBoth && LA_isDoubleLex(&bothstring)){
                Lexicon nl = newLexicon(bothstring.ptr, bothstring.ptr);
                ret.add(&ret, &nl, sizeof(Lexicon));
                index++;    
            }else if(singles.findc(&singles, c)){
                Lexicon nl = newLexicon(cs, cs);
                ret.add(&ret, &nl, sizeof(Lexicon));
            }else if(numbers.findc(&numbers, c)){
                temp.addc(&temp, c);
                int dotCount = 0;
                c = sf.ptr[++index];
                while(numbers.findc(&numbers, c)){
                    temp.addc(&temp, c);
                    c = sf.ptr[++index];
                }
                index--;
                Lexicon nl = newLexicon("num", temp.ptr);
                ret.add(&ret, &nl, sizeof(nl));
                temp.setcs(&temp, "");
            }
            
        }else if(state == 1){ //continuing on for names of variables or program key words
            if(names.findc(&names, c) || numbers.findc(&numbers, c)){
                temp.addc(&temp, c);
            }else{
                index--;
                Lexicon nl;
                if(temp.eqcs(&temp, "print"))
                    nl = newLexicon("print", "print");
                else nl = newLexicon("var", temp.ptr);
                ret.add(&ret, &nl, sizeof(Lexicon));
                temp.setcs(&temp, "");
                
                state = 0;
            }
        }
        
        index++;
        bothstring.del(&bothstring);
    }
    
    numbers.del(&numbers);
    names.del(&names);
    temp.del(&temp);
    singles.del(&singles);
    
    return ret;
}

LStr getFile(char * fname){
    LStr ret = newLStr();
    FILE * fi = fopen(fname, "r");
    char buf[101];
    while(1){
        int nr = fread(buf, 1, 100, fi);
        buf[nr] = 0x00;
        if(nr <= 0) break;
        ret.addcs(&ret, buf);
    }
    fclose(fi);
    return ret;
}

int main(){
    
    LStr file = getFile("script1");
    LList lexList = lexicalAnalysis(file);
    for(int i = 0; i < lexList.length; i++){
        Lexicon * l = (Lexicon*)lexList.get(&lexList, i);
        if(l->type.eqcs(&l->type, "num")) printf("%s, %s\n", l->type.ptr, l->val.ptr);
        else printf("%s\n", l->type.ptr);
    }
    
 
    
    return 0;
}