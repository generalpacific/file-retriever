
#ifndef _stop
#define _stop 1

#include"header.h"


/*
3d Array for storing the stop words.
stopWord[i][j][k] where i is the index for starting letter of each word.

Future development : Use of trie.
*/

int checkForStopWords(char *);
void initializeStopWordsArray();

#endif
