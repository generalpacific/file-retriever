
#ifndef _vector
#define _vector 1

#include"header.h"

typedef struct _termVector{
	char term[MAXIMUM];
	float tfidf;
	struct _termVector *next; 
}termVector;

typedef struct _doc {
	int docId;
	float score;
	termVector *termVectorPtrHead;
	termVector *termVectorPtrEnd;
	struct _doc *next;

}docVector;

docVector* getDocVectorNode();
termVector* getTermVectorNode();

void updateDocVectorDocId();
void updateTermVectorInDocVector();

void calculateScore();
void printDocVector();
void writeDocVectorToFile();

#endif
