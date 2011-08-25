

#include"term.h"

/**************************
	Functions related to trie
**************************/

Trie *trieRoot = NULL;

double fileMag[MAXDOCS];
int fileLen[MAXDOCS];
int numOfDocs;


void initializeTrie() {
	trieRoot = getTrieNode(); 
}


Trie* getTrieNode() {
	Trie *temp;

	temp = (Trie*)malloc(sizeof(Trie));
	if(temp == NULL){
		return NULL;
	}
	temp->isTermFlag = FAIL;

	int i;
	for(i = 0; i < 26; ++i) {
		temp->next[i] = NULL;
	}

	return temp;

}

void traverseTrie(Trie *trieNode) {
	
	int i;
	
	if(trieNode == NULL)
		return;

	if(trieNode->isTermFlag == SUCCESS) {
		printf("%s\t\t$ ",trieNode->termBlock->term);
		printf("%d $ ",trieNode->termBlock->df);
		Postings *tempPostingsNode;
		tempPostingsNode = trieNode->termBlock->post;
		while(tempPostingsNode != NULL) {
			printf( "%d:%d-> ",tempPostingsNode->docId,tempPostingsNode->tf);
			tempPostingsNode = tempPostingsNode->next;
		}
		printf("\n");
	}

	for(i = 0; i < 26; ++i) {
		traverseTrie(trieNode->next[i]);
	}

}

void traverseTerms() {
	traverseTrie(trieRoot);
}


/**************************
	Functions related to index building
**************************/
void traverseTrieForIndexFile(Trie *trieNode) {
	
	int i;
	double tempMag;
	
	if(trieNode == NULL)
		return;

	if(trieNode->isTermFlag == SUCCESS) {
	
		FILE  *indexFp;

		char indexFile[MAXIMUM],temp[3];
		strcpy(indexFile,"INDEX/");
		

		temp[0] = trieNode->termBlock->term[0];
		temp[1] = '/';
		temp[2] = '\0';
		strcat(indexFile,temp);

		temp[1] = trieNode->termBlock->term[1];
		temp[2] = '\0';
		strcat(indexFile,temp);

		indexFp = fopen(indexFile,"a");

		fprintf(indexFp,"%s$",trieNode->termBlock->term);
		fprintf(indexFp,"%d$",trieNode->termBlock->df);

		

		Postings *tempPostingsNode;
		tempPostingsNode = trieNode->termBlock->post;
		while(tempPostingsNode != NULL) {
		
			if(strcmp("prashant",trieNode->termBlock->term)==0){
				printf("Indexing %s to %d\n",trieNode->termBlock->term,tempPostingsNode->docId);
			}

			fprintf(indexFp,"%d:%d#",tempPostingsNode->docId,tempPostingsNode->tf);

			tempMag = ((float)tempPostingsNode->tf / (float)fileLen[tempPostingsNode->docId])* log((double)numOfDocs/(double)trieNode->termBlock->df);
			fileMag[tempPostingsNode->docId] = fileMag[tempPostingsNode->docId] + pow(tempMag,2);


			tempPostingsNode = tempPostingsNode->next;
		}
		lseek(fileno(indexFp),-1,SEEK_CUR);
		fprintf(indexFp,"\n");

		fclose(indexFp);
	}

	for(i = 0; i < 26; ++i) {
		traverseTrieForIndexFile(trieNode->next[i]);
	}

}

void createIndexFile() {


	traverseTrieForIndexFile(trieRoot);

	FILE *fpMagFile;
	fpMagFile = fopen("INDEX/filesmag.txt","w");

	int i;
	for(i = 1; i <= numOfDocs; ++i) {
		fileMag[i] = sqrt(fileMag[i]);
	}

	for(i = 1; i <= numOfDocs; ++i) {
		fprintf(fpMagFile,"%lf %d\n",fileMag[i],fileLen[i]);
	}


	fclose(fpMagFile);
}


/**************************
	Functions related to Postings
**************************/

Postings* getPostingsNode() {

	Postings *tempPostingsNode;

	tempPostingsNode = (Postings*)malloc(sizeof(Postings));
	tempPostingsNode->next = NULL;
	tempPostingsNode->tf = 1;

	return tempPostingsNode;

}

int updatePostings(Postings *head,int docId) {

	Postings *tempPostingsNode, *tempPostingsNode2;

	tempPostingsNode = head;

/*	while(tempPostingsNode != NULL) {
		if(tempPostingsNode->docId == docId) {
			tempPostingsNode->tf++;
			return FAIL;
		}
		tempPostingsNode2 = tempPostingsNode;
		tempPostingsNode = tempPostingsNode->next;
	}

	tempPostingsNode = getPostingsNode();
	tempPostingsNode->docId = docId;
	tempPostingsNode2->next = tempPostingsNode;
	return SUCCESS;*/

	while(tempPostingsNode != NULL) {
		if(tempPostingsNode->docId == docId) {
			tempPostingsNode->tf++;
			return FAIL;
		}
		
		if(tempPostingsNode->docId > docId) {
			if(tempPostingsNode == head) {
				Postings *newPostingsNode = getPostingsNode();
				newPostingsNode->docId = docId;
				newPostingsNode->next = head;
				head = newPostingsNode;
				return SUCCESS;
			}
			else {
				Postings *newPostingsNode = getPostingsNode();
				newPostingsNode->docId = docId;
				newPostingsNode->next = tempPostingsNode;
				tempPostingsNode2->next = newPostingsNode;
				return SUCCESS;
			}
		}

		tempPostingsNode2 = tempPostingsNode;
		tempPostingsNode = tempPostingsNode->next;
	}
	
	Postings *newPostingsNode = getPostingsNode();
	newPostingsNode->docId = docId;
	newPostingsNode->next = NULL;
	tempPostingsNode2->next = newPostingsNode;
	return SUCCESS;
}

/**************************
	The addTerm function
**************************/

void addTerm(char *word, int docId) {
	
	int i = 0;
	int letterNumber;

	Trie *tempTrieNode,*newTrieNode;
	tempTrieNode = trieRoot;

	/*if(trieRoot == NULL){
		printf("ERROR : malloc has failed. Cannot process further files.\nStoring Index now....\n");

		docId++;
		numOfDocs = docId - 1;
		traverseTerms();
		createIndexFile();

		exit(0);
	}*/


	while(word[i] != '\0') {
		letterNumber = word[i] - 97;

		
		if(tempTrieNode->next[letterNumber] == NULL) {
			newTrieNode = getTrieNode();
			if(newTrieNode == NULL){
				printf("ERROR : malloc has failed. Cannot process further files.\nStoring Index now....\n");

				docId++;
				numOfDocs = docId - 1;
				traverseTerms();
				createIndexFile();

				exit(0);
			}
			tempTrieNode->next[letterNumber] = newTrieNode;
			tempTrieNode = newTrieNode;

		}
		else {
			tempTrieNode = tempTrieNode->next[letterNumber];
	/*		if(tempTrieNode == NULL){
				printf("ERROR : malloc has failed. Cannot process further files.\nStoring Index now....\n");

				docId++;
				numOfDocs = docId - 1;
				traverseTerms();
				createIndexFile();

				exit(0);
			}*/
		}
		++i;

	}

	if(strcmp("prashant",word)==0){
		printf("Adding %s to %d\n",word,docId);
	}

	if(tempTrieNode->isTermFlag == SUCCESS) {

		if(updatePostings(tempTrieNode->termBlock->post,docId) == SUCCESS) {
			// new node added; Update the df
			tempTrieNode->termBlock->df += 1;
		}

	}
	else {
		tempTrieNode->isTermFlag = SUCCESS;
		tempTrieNode->termBlock = (Terms*)malloc(sizeof(Terms));

		int len = strlen(word);
		tempTrieNode->termBlock->term = (char*)malloc(len+1);
		strcpy(tempTrieNode->termBlock->term,word);
		tempTrieNode->termBlock->df = 1; 
		
		Postings *tempPostingsNode;

		tempPostingsNode = getPostingsNode();
		tempTrieNode->termBlock->post = tempPostingsNode;
		tempTrieNode->termBlock->post->docId = docId;
		
	}
}

/*void addTerm(char *word, int updateDf, int docId) {

	if(termHeader == NULL) {
		termHeader = (Terms*)malloc(sizeof(Terms));

		strcpy(termHeader->term,word);
		termHeader->df = 1;

		Postings *tempPosting;
		tempPosting = (Postings*)malloc(sizeof(Postings));

		tempPosting->docId = docId;
		tempPosting->tf = 1;
		tempPosting->next = NULL;

		termHeader->post = tempPosting;
		termHeader->next = NULL;

		return;
	}

	Terms *tempTerm1, *tempTerm2;

	tempTerm1 = termHeader;

	while(tempTerm1 != NULL) {


		if(strcmp(tempTerm1->term,word) == 0) {
//			updatePosting(tempTerm1,docId);
		}


		if(strcmp(tempTerm1->term,word) > 0) {
			// insert word before tempTerm1 as term > word
			
			if(tempTerm1 == termHeader) {
				// add new termHeader

				Terms *newTerm;
				newTerm = (Terms*)malloc(sizeof(Terms));

				strcpy(newTerm,word);
				newTerm->df = 1;

				Postings *tempPosting;
				tempPosting = (Postings*)malloc(sizeof(Postings));

				tempPosting->docId = docId;
				tempPosting->tf = 1;
				tempPosting->next = NULL;

				newTerm->post = tempPosting;
				newTerm->next = NULL;

				newTerm->next = termHeader;
				termHeader = newTerm;
				return;

			}


		}

	}

}*/
