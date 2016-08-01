#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H

#include "LStr.h"


typedef struct LexiconStruct{
    LStr val;
    LStr type;
    
    
    void (*del)(struct LexiconStruct *);
} Lexicon;

Lexicon newLexicon(char * t, char * v);


int LA_isDoubleLex(LStr * st);
int LA_contains(LList * list, LStr * st);


#endif