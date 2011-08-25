
#ifndef _query
#define _query 1

#include"header.h"
#include"search.h"
#include"stack.h"
#include"boolean.h"


int operatorCount;

void removeStopWordsFromQuery(char *query);
void evaluateQuery(char *query);

#endif
