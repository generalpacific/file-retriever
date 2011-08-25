
#ifndef _term
#define _term 1

#include"header.h"
/*
Linked list for storing the postings
*/

typedef struct _postings {
	int docId;
	int tf;
	struct _postings *next;
}Postings;

/* termBlock if isTermFlag = TRUE in trieNode */

typedef struct _terms {
	char *term;
	int df;
	Postings *post;
}Terms;

/*
Trie for storing terms.
*/

typedef struct _trie {
	int isTermFlag;
	Terms *termBlock;
	struct _trie *next[26];
}Trie;

void addTerm(char *word, int docId);

/* trie functions */
void initializeTrie();
Trie* getTrieNode();
void traverseTrie(Trie*);
void traverseTerms();

/* postings functions */
Postings *getPostingsNode();
int updatePostings(Postings *,int docId);

/* index file creation functions */
void createIndexFile();
void traverseTrieForIndexFile(Trie *);

#endif
