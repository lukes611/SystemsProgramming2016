#include <stdio.h>
#include <stdlib.h>
#include "LList.h"
#include "LStr.h"
#include "LexicalAnalysis.h"

/*

parser:

*/
typedef struct{
    int index;
    LList l;
    LList ass;
    int tIndex;
    LList garb;
}Parser;

LStr ParserElem(Parser * self);
LStr ParserMFac(Parser * self, LStr * varA);
LStr ParserFac(Parser * self);
LStr ParserMAdd(Parser * self, LStr * varA);
LStr ParserAdd(Parser * self);
LStr ParserUnary(Parser * self);
LStr ParserMComp(Parser * self, LStr * varA);
LStr ParserComp(Parser * self);
LStr ParserAND(Parser * self);
LStr ParserMAND(Parser * self, LStr * varA);
LStr ParserOR(Parser * self);
LStr ParserLHS(Parser * self);
LStr ParserMOR(Parser * self, LStr * varA);
int ParserAss(Parser * self);

void ParserInit(Parser * self, LList lexList){
    self->index = 0;
    self->l = lexList;
    self->ass = newLList();
    self->tIndex = 0;
    self->garb = newLList();
}

void ParserPrint(Parser * self){
    int i = 0;
    for(i = 0; i < self->ass.length; i++){
        LStr * s = (LStr *) self->ass.get(&self->ass, i);
        s->print(s);
    }
}


int ParserStmt(Parser * self){
    if(self->index >= self->l.length) return 0;
    Lexicon * l1 = (Lexicon *)self->l.get(&self->l, self->index);
    if(ParserAss(self)){
        self->index++;
        ParserStmt(self);
    }else if( l1->type.eqcs(&l1->type, ";") ){
        self->index++;
        ParserStmt(self);
    }else if(
        l1->type.eqcs(&l1->type, "var")
        ||l1->type.eqcs(&l1->type, "str")
        ||l1->type.eqcs(&l1->type, "num")
        ||l1->type.eqcs(&l1->type, "not")
        ||l1->type.eqcs(&l1->type, "-")
    ){
        
        ParserOR(self);
        ParserStmt(self);
        
    }
    return 0;
}

int ParserAss(Parser * self){
    int i1 = self->index, i2 = self->index+1;
    if(self->index+1 >= self->l.length) return 0;
    Lexicon * l1 = (Lexicon *)self->l.get(&self->l, i1);
    Lexicon * l2 = (Lexicon *)self->l.get(&self->l, i2);
    if(l1->type.eqcs(&l1->type, "var") &&
      (
        l2->type.eqcs(&l2->type, "=")
        ||l2->type.eqcs(&l2->type, "+=")
        ||l2->type.eqcs(&l2->type, "*=")
          ||l2->type.eqcs(&l2->type, "-=")
          ||l2->type.eqcs(&l2->type, "/=")
          ||l2->type.eqcs(&l2->type, "%=")
      
      )){
        LStr v1 = ParserLHS(self);
        self->index++;
        LStr v2 = ParserOR(self);
        LStr as = newLStr();
        as.set(&as, &l2->type); as.addcs(&as, " ");
        as.add(&as, &v1); as.addcs(&as, " "); as.add(&as, &v2);
        self->ass.add(&self->ass, &as, sizeof(as));
        
        return 1;
    }
    return 0;
}

LStr ParserOR(Parser * self){
    LStr var = ParserAND(self);
    return ParserMOR(self, &var);
}

LStr ParserMOR(Parser * self, LStr * varA){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "or")){
        self->index++;
        LStr varB = ParserAND(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        //add ass
        LStr as = newLStr();
        as.set(&as, &l->type); as.addcs(&as, " "); as.add(&as, &temp);
        as.addcs(&as, " "); as.add(&as, varA); as.addcs(&as, " "); as.add(&as, &varB);
        self->ass.add(&self->ass, &as, sizeof(as));
        return ParserMOR(self, &temp);
    }
    return *varA;
}


LStr ParserAND(Parser * self){
    LStr var = ParserComp(self);
    return ParserMAND(self, &var);
}

LStr ParserMAND(Parser * self, LStr * varA){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "and")){
        self->index++;
        LStr varB = ParserComp(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        //add ass
        LStr as = newLStr();
        as.set(&as, &l->type); as.addcs(&as, " "); as.add(&as, &temp);
        as.addcs(&as, " "); as.add(&as, varA); as.addcs(&as, " "); as.add(&as, &varB);
        self->ass.add(&self->ass, &as, sizeof(as));
        return ParserMAND(self, &temp);
    }
    return *varA;
}

LStr ParserComp(Parser * self){
    LStr var = ParserAdd(self);
    return ParserMComp(self, &var);
}

LStr ParserMComp(Parser * self, LStr * varA){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "<")
      ||l->type.eqcs(&l->type, ">")
       ||l->type.eqcs(&l->type, "<=")
       ||l->type.eqcs(&l->type, "!=")
       ||l->type.eqcs(&l->type, ">=")
       ||l->type.eqcs(&l->type, "==")
      
      ){
        self->index++;
        LStr varB = ParserAdd(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        //add ass
        LStr as = newLStr();
        as.set(&as, &l->type); as.addcs(&as, " "); as.add(&as, &temp);
        as.addcs(&as, " "); as.add(&as, varA); as.addcs(&as, " "); as.add(&as, &varB);
        self->ass.add(&self->ass, &as, sizeof(as));
        return ParserMComp(self, &temp);
    }
    return *varA;
}

LStr ParserAdd(Parser * self){
    LStr var = ParserFac(self);
    return ParserMAdd(self, &var);
}

LStr ParserMAdd(Parser * self, LStr * varA){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "+") || l->type.eqcs(&l->type, "-")){
        self->index++;
        LStr varB = ParserFac(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        //add ass
        LStr as = newLStr();
        as.set(&as, &l->type); as.addcs(&as, " "); as.add(&as, &temp);
        as.addcs(&as, " "); as.add(&as, varA); as.addcs(&as, " "); as.add(&as, &varB);
        self->ass.add(&self->ass, &as, sizeof(as));
        return ParserMAdd(self, &temp);
    }
    return *varA;
}

LStr ParserFac(Parser * self){
    LStr var = ParserUnary(self);
    return ParserMFac(self, &var);
}

LStr ParserMFac(Parser * self, LStr * varA){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "*") || l->type.eqcs(&l->type, "/") || l->type.eqcs(&l->type, "%")){
        self->index++;
        LStr varB = ParserUnary(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        //add ass
        LStr as = newLStr();
        as.set(&as, &l->type); as.addcs(&as, " "); as.add(&as, &temp);
        as.addcs(&as, " "); as.add(&as, varA); as.addcs(&as, " "); as.add(&as, &varB);
        self->ass.add(&self->ass, &as, sizeof(as));
        return ParserMFac(self, &temp);
    }
    return *varA;
}

LStr ParserUnary(Parser * self){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index);
    if(l->type.eqcs(&l->type, "-") || l->type.eqcs(&l->type, "not")){
        self->index++;
        LStr var = ParserElem(self);
        LStr temp = newLStr(); temp.setcs(&temp, "t"); temp.addi(&temp, self->tIndex++);
        LStr assembly = newLStr();
        
        if(l->type.eqcs(&l->type, "-")) assembly.setcs(&assembly, "neg "); 
        else assembly.setcs(&assembly, "not "); 
        
        assembly.add(&assembly, &temp); assembly.addcs(&assembly, " "); assembly.add(&assembly, &var);
        self->ass.add(&self->ass, &assembly, sizeof(LStr));
        return temp;
    }
    return ParserElem(self);    
}

LStr ParserElem(Parser * self){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index++);
    LStr ret = newLStr();
    if(l->type.eqcs(&l->type, "num") || l->type.eqcs(&l->type, "var") || l->type.eqcs(&l->type, "str")){
        ret.set(&ret, &l->val);
    }
    return ret;
}

LStr ParserLHS(Parser * self){
    Lexicon * l = (Lexicon *)self->l.get(&self->l, self->index++);
    LStr ret = newLStr();
    if(l->type.eqcs(&l->type, "var")){
        ret.set(&ret, &l->val);
    }
    return ret;
}





int main(){
    
    LStr file = LA_file("script1");
    LList lexList = LA_lexicalAnalysis(file);
    for(int i = 0; i < lexList.length; i++){
        Lexicon * l = (Lexicon*)lexList.get(&lexList, i);
        int pb = l->type.eqcs(&l->type, "num") || l->type.eqcs(&l->type, "var") || l->type.eqcs(&l->type, "str");
        
        //if(pb) printf("%s, %s\n", l->type.ptr, l->val.ptr);
        //else printf("%s\n", l->type.ptr);
    }
    
    Parser p;
    ParserInit(&p, lexList);
    //LStr ret = ParserOR(&p);
    ParserStmt(&p);
    //ret.print(&ret);
    
    ParserPrint(&p);
 
    
    return 0;
}