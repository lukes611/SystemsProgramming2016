#include "LexicalAnalysis.h"
#include "LList.h"
#include <stdio.h>

static void __del(Lexicon * self){
    self->type.del(&self->type);
    self->val.del(&self->val);
}

Lexicon newLexicon(char * t, char * v){
    Lexicon ret;
    ret.val = newLStr();
    ret.val.setcs(&ret.val, v);
    ret.type = newLStr();
    ret.type.setcs(&ret.type, t);
    ret.del = __del;
    return ret;
}

int LA_contains(LList * list, LStr * st){
    int result = 0, i;
    for(i = 0; i < list->length; i++){
        LStr * ptr = list->get(list, i);
        if(ptr->eq(ptr, st)){
            result = 1;
            break;
        }
    }
    return result;
}

int LA_isDoubleLex(LStr * st){
    LStr _ = newLStr(); _.setcs(&_, "!= == <= >= += -= *= /= %= ++ --");
    LList list = _.split(&_, ' ');
    int result = LA_contains(&list, st);
    int i;
    for(i = 0; i < list.length; i++){
        LStr * ptr = list.get(&list, i);
        ptr->del(ptr);
    }
        
    
    _.del(&_);
    list.del(&list);
    
    return result;
}

int LA_isKeyWord(LStr * st){
    LStr _ = newLStr(); _.setcs(&_, "print if else func class for break continue and or not");
    LList list = _.split(&_, ' ');
    int result = LA_contains(&list, st);
    int i;
    for(i = 0; i < list.length; i++){
        LStr * ptr = list.get(&list, i);
        ptr->del(ptr);
    }
        
    
    _.del(&_);
    list.del(&list);
    
    return result;
}


LList LA_lexicalAnalysis(LStr sf){
    LList ret = newLList();
    
    sf.addc(&sf, ';');
    int index = 0;
    int state = 0;
    LStr names = newLStr(); names.setcs(&names, "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP$");
    LStr numbers = newLStr(); numbers.setcs(&numbers, "0123456789.");
    LStr singles = newLStr(); singles.setcs(&singles, "+*-/=!()[].%{}<>");
    char stringStart = '"';
    
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
            }else if(c == '\'' || c == '"'){
                temp.setcs(&temp, "");
                stringStart = c;
                state = 2;
            }
            
        }else if(state == 1){ //continuing on for names of variables or program key words
            if(names.findc(&names, c) || numbers.findc(&numbers, c) && c != '.'){
                temp.addc(&temp, c);
            }else{
                index--;
                Lexicon nl;
                if(LA_isKeyWord(&temp))
                    nl = newLexicon(temp.ptr, temp.ptr);
                else nl = newLexicon("var", temp.ptr);
                ret.add(&ret, &nl, sizeof(Lexicon));
                temp.setcs(&temp, "");
                
                state = 0;
            }
        }else if(state == 2){
            if(c == stringStart){
                Lexicon nl = newLexicon("str", temp.ptr);
                ret.add(&ret, &nl, sizeof(nl));
                state = 0;
                temp.setcs(&temp, "");
            }else if(c == '\\'){
                if(canHaveBoth){
                    temp.addc(&temp, both[1]);
                    index++;
                }
            }else{
                temp.addc(&temp, c);
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

LStr LA_file(char * fname){
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
