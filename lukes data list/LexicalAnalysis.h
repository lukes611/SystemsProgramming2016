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

int LA_isKeyWord(LStr * st);

LList LA_lexicalAnalysis(LStr sf);

LStr LA_file(char * fname);


#endif