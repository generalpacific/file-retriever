

#include"search.h"

extern Result* popResult();
extern void pushResult(Result *);

//char docs[MAX][MAX];


int totalNumDoc;

void initializeDocIdArray() {


	int i;
	FILE *files,*conf;
	char tempFileName[MAXIMUM],filesName[MAXIMUM];

/*	conf = fopen("CONFIG","r");
	readLine(conf,filesName);
	fclose(conf);*/

	files = fopen("INDEX/files.txt","r");
	//printf("%s\n",filesName);

	int docId = 1;
	while(readLine(files,tempFileName) != FAIL) {
		//printf("%s \n",tempFileName);
		strcpy(docs[docId].name,tempFileName);
		++docId;
	}
	totalNumDoc = docId - 1;

	fclose(files);

	files = fopen("INDEX/filesmag.txt","r");


	for(i = 1; i < docId; ++i){
		fscanf(files,"%f %d\n",&docs[i].fileMag,&docs[i].fileLen);
	}
	fclose(files);

	/* for testing */
/*	
	for(i = 1; i < docId; ++i) {
		printf("%s %d %lf\n",docs[i].name,docs[i].fileLen,docs[i].fileMag);
	}
	printf("\n");*/
}

void printResult(){

	Result *finalResult;
	finalResult = popResult();
	pushResult(finalResult);

	printf("In print result\n");

	while(finalResult != NULL) {
		printf("%d %s\n",finalResult->docId,docs[finalResult->docId].name);
		finalResult = finalResult->next;
	}
}


int searchQueryInIndex(char *query) {
	
	FILE *indexFp;
	struct stat sb;

	char indexFile[MAXSMALL], temp[3], term[MAXSMALL];
	char entry[MAXBIG];

	int i;

	strcpy(indexFile,"INDEX/");

	temp[0] = query[0];
	temp[1] = '/';
	temp[2] = '\0';
	strcat(indexFile,temp);

	temp[1] = query[1];
	strcat(indexFile,temp);

	if(stat(indexFile,&sb) == -1)	{
		return FAIL;
	}

	indexFp = fopen(indexFile,"r");


	printf("\nResult : \n");

	while(readLine(indexFp,entry) != FAIL) {


		/* getting term */
		i = 0;
		while(entry[i] != '$') {
			term[i] = entry[i];
			++i;
		}
		term[i] = '\0';

		if(strcmp(term,query) != 0)
			continue;
		
		/*getting df */
		++i;
		char dfChar[10];
		int j = 0,df;
		while(entry[i] != '$') {
			dfChar[j] = entry[i];
			++i;
			++j;
		}
		dfChar[j] = '\0';
		df = atoi(dfChar);

		/* Getting the postings */
		++i;
		int k = 0;
		while(k < df) {
			char tempChar[10];
			
			j = 0;
			while(entry[i] != ':') {
				tempChar[j] = entry[i];
				++i;
				++j;	
			}
			tempChar[j] = '\0';
			printf("%s\n",docs[atoi(tempChar)].name);

			++i;
			j = 0;
			while(entry[i] != '#') {
				tempChar[j] = entry[i];
				++j;
				++i;
			}
			tempChar[j] = '\0';
			//printf("%d#",atoi(tempChar));
			//printf("%s#",tempChar);

			++i;
			++k;
		}

		printf("\n");
		break;
	}
}

int searchTermUpdateStack(char *query) {
	
	printf("In searchTermUpdateStack..\n");

	FILE *indexFp;
	struct stat sb;

	char indexFile[MAXSMALL], temp[3], term[MAXSMALL];
	char entry[MAXSMALL];

	Result *headResult,*endResult;
	headResult = NULL;
	endResult = NULL;

	int i;

	strcpy(indexFile,"INDEX/");

	temp[0] = query[0];
	temp[1] = '/';
	temp[2] = '\0';
	strcat(indexFile,temp);

	temp[1] = query[1];
	strcat(indexFile,temp);

	if(stat(indexFile,&sb) == -1)	{
		return FAIL;
	}

	indexFp = fopen(indexFile,"r");


	while(readTerm(indexFp,term) != FAIL) {

//		printf("readTerm : %s --\n",term);

		/* getting term */

		if(strcmp(term,query) != 0) {
			skipToNewLine(indexFp);
			continue;
		}
		
		/*getting df */
		int df = getDf(indexFp);

		/* Getting the postings */
		++i;
		int k = 0;
		while(k < df) {
			char tempChar[10];

			getRecord(indexFp,entry);
//			printf("entry : %s\n",entry);
			
			int j = 0;
			i = 0;
			while(entry[i] != ':') {
				tempChar[j] = entry[i];
				++i;
				++j;	
			}
			tempChar[j] = '\0';
	
			if(headResult == NULL) {
				headResult = (Result*)malloc(sizeof(Result));
				headResult->docId = atoi(tempChar);
				headResult->next = NULL;
				endResult = headResult;
			}
			else{
				Result *newResultHead;
				newResultHead = (Result*)malloc(sizeof(Result));
				newResultHead->docId = atoi(tempChar);
				newResultHead->next = NULL;
				endResult->next = newResultHead;
				endResult = endResult->next;
			}
			//printf("%s\n",docs[atoi(tempChar)]);

			++i;
			j = 0;
			while(entry[i] != '\0') {
				tempChar[j] = entry[i];
				++j;
				++i;
			}
			tempChar[j] = '\0';
			//printf("%d#",atoi(tempChar));
			//printf("%s#",tempChar);

			++i;
			++k;
		}

		break;
	}

	pushResult(headResult);
}


/* This function is not used presently */
void createIndexFromFile() {

	FILE *indexFp;
	indexFp = fopen("index.txt","r");
	char entry[MAXBIG],term[MAXIMUM];

	int i;

	while(readLine(indexFp,entry) != FAIL) {


		/* getting term */
		i = 0;
		while(entry[i] != '$') {
			term[i] = entry[i];
			++i;
		}
		term[i] = '\0';
		printf("Term : %s\n",term);
		
		/*getting df */
		++i;
		char dfChar[10];
		int j = 0,df;
		while(entry[i] != '$') {
			dfChar[j] = entry[i];
			++i;
			++j;
		}
		dfChar[j] = '\0';
		df = atoi(dfChar);
		printf("df : %d\n",df);

		/* Getting the postings */
		++i;
		int k = 0;
		while(k < df) {
			char tempChar[10];
			
			j = 0;
			while(entry[i] != ':') {
				tempChar[j] = entry[i];
				++i;
				++j;	
			}
			tempChar[j] = '\0';
			printf("%d:",atoi(tempChar));

			++i;
			j = 0;
			while(entry[i] != '#') {
				tempChar[j] = entry[i];
				++j;
				++i;
			}
			tempChar[j] = '\0';
			//printf("%d#",atoi(tempChar));
			printf("%s#",tempChar);

			++i;
			++k;
		}

	}
}
