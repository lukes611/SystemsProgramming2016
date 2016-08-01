#include "LexicalAnalysis.h"
#include "LList.h"

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
    LStr _ = newLStr(); _.setcs(&_, "!= == <= >= += -= *= /= %=");
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