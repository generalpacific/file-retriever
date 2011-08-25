
#ifndef _search
#define _search 1

#include"header.h"
typedef struct _documents{
	char name[MAXIMUM];
	int fileLen;
	float fileMag;
}documents;

documents docs[MAXDOCS];

typedef struct _result {
	int docId;
	struct _result *next;
}Result;

void intializeDocIdArray();
void createIndexFromFile(); //is not going to use it for a while

int searchQueryInIndex(char *);
void printResult();

int searchTermUpdateStack(char *);

#endif
