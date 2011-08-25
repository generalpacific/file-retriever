
#ifndef _stack
#define _stack 1

#include"header.h"
#include"search.h"


typedef struct _stackNode {
	Result *head;
	struct _stackNode *next;
}stackNode;

void initializeStack();
void pushResult(Result *);
Result* popResult();
int emptyStack();
void printStack();



#endif
