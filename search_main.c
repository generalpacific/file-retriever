/*	Author 	: Chaudhary Prashant
	Desc 	: The main function searching query in the index.
	License	: GPLv3
*/



#include"header.h"



int main(int argc, char **argv) {
	char query[MAXIMUM];	

	/*initializeStopWordsArray();
	initializeTrie();*/

	printf("Initializing Documents Structure......\n");	
	initializeDocIdArray();

	printf("Initializing Stop Words Array......\n");
	initializeStopWordsArray();
	initializeStack();

	//createIndexFromFile();

	
//	printf("argv = %s \n",argv[1]);
	strcpy(query, argv[1]);

	printf("Processing Query......\n");
	convertToLowerCase(query);
	removeSpecialSymbols(query);
	removeStopWordsFromQuery(query);
	printf("Processed Query : %s\n",query);
//	printf("Processed Query : %s\n",query);

	printf("Evaluating Query.....\n");
	evaluateQuery(query);

	//printResult();

	getIndexTerms(query);

	updateDocVectorDocId();
	updateTermVectorInDocVector();
	calculateScore();
	//printDocVector();

	printf("\nAfter ranking\n");
	sortDocVector();
	printDocVector();
	writeDocVectorToFile();

//	searchQueryInIndex(query);

}


