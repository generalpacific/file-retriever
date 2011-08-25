

#include"vectorspace.h"
#include"search.h"

docVector *head;
extern Result* popResult();
extern char indexTermsInQuery[MAXIMUM][MAXIMUM];
extern int indexTermsInQueryCount;
extern int totalNumDoc;


docVector* getDocVectorNode() {
	docVector *temp;
	
	temp = (docVector*)malloc(sizeof(docVector));
	temp->next = NULL;
	temp->termVectorPtrHead = NULL;
	temp->termVectorPtrEnd = NULL;

	return temp;
}

void updateDocVectorDocId() {

	printf("In updateDocVectorDocId..\n");

	docVector *end;
	Result *booleanResult;
	booleanResult = popResult();

	while(booleanResult != NULL) {
		if(head == NULL) {
			head = getDocVectorNode();
			end = head;
			head->docId = booleanResult->docId;
		}
		else {
			docVector *temp;
			temp = getDocVectorNode();
			temp->docId = booleanResult->docId;
			
			end->next = temp;
			end = temp;
		}
		booleanResult = booleanResult->next;
	}
	

}

termVector* getTermVectorNode() {
	termVector *temp;

	temp = (termVector*)malloc(sizeof(termVector));
	temp->next = NULL;
	temp->term[0] = '\0';
	temp->tfidf = 0;

	return temp;
}

void updateTermVectorInDocVector() {
	
	printf("In updateTermVectorInDocVector..\n");

	int cnt;

	for(cnt = 0; cnt < indexTermsInQueryCount; ++cnt) {
	
		FILE *indexFp;
		struct stat sb;

		char indexFile[MAXSMALL], temp[3], term[MAXSMALL];
		char entry[MAXBIG];
		char indexTerm[MAXBIG];

		docVector *tempDocVector;
		tempDocVector = head;

		strcpy(indexTerm,indexTermsInQuery[cnt]);

		int i;

		strcpy(indexFile,"INDEX/");

		temp[0] = indexTerm[0];
		temp[1] = '/';
		temp[2] = '\0';
		strcat(indexFile,temp);

		temp[1] = indexTerm[1];
		strcat(indexFile,temp);

		if(stat(indexFile,&sb) == -1)	{
			continue;
		}

		indexFp = fopen(indexFile,"r");

		while(readTerm(indexFp,term) != FAIL) {

//			printf("term = %s--\n",term);

			if(strcmp(term,indexTerm) != 0){
				skipToNewLine(indexFp);
				continue;
			}
			
			/*getting df */
			int df = getDf(indexFp);

			/* Getting the postings */
			int k = 0;
			while(k < df) {
				char tempChar[10];
			
				getRecord(indexFp, entry);
				
				int j = 0;
				i = 0;
				while(entry[i] != ':') {
					tempChar[j] = entry[i];
					++i;
					++j;	
				}
				tempChar[j] = '\0';
				int docId = atoi(tempChar);


				++i;
				j = 0;
				while(entry[i] != '\0') {
					tempChar[j] = entry[i];
					++j;
					++i;
				}
				tempChar[j] = '\0';
				int tf = atoi(tempChar);
				//printf("%d#",atoi(tempChar));
				//printf("%s#",tempChar);
				
				while(tempDocVector != NULL && tempDocVector->docId < docId) 
					tempDocVector = tempDocVector->next;

				if(tempDocVector != NULL && tempDocVector->docId == docId) {
					if(tempDocVector->termVectorPtrHead == NULL) {
						tempDocVector->termVectorPtrHead = getTermVectorNode();
						strcpy(tempDocVector->termVectorPtrHead->term,indexTerm);

						tempDocVector->termVectorPtrHead->tfidf = ((float)tf/(float)docs[docId].fileLen)*log((float)totalNumDoc/(float)df); /***** wt set here *****/

						tempDocVector->termVectorPtrEnd = tempDocVector->termVectorPtrHead;
					}
					else {
						termVector *tempTermVector;
						tempTermVector = getTermVectorNode();
						strcpy(tempTermVector->term,indexTerm);

						tempTermVector->tfidf = ((float)tf/(float)docs[docId].fileLen)*log((float)totalNumDoc/(float)df); /***** wt set here *****/

						tempDocVector->termVectorPtrEnd->next = tempTermVector;
						tempDocVector->termVectorPtrEnd = tempTermVector;
					}
				}

				++i;
				++k;
			}

			fclose(indexFp);
			break;
		}
	}
}

void calculateScore() {
	int i;
	
	docVector *tempDocVector;
	tempDocVector = head;

	while(tempDocVector != NULL) {
		float score = 0;
		for(i = 0; i < indexTermsInQueryCount; ++i) {

			termVector *tempTermVector;
			tempTermVector = tempDocVector->termVectorPtrHead;

			while(tempTermVector != NULL) {
				if(strcmp(indexTermsInQuery[i],tempTermVector->term) == 0) {
					score = score + tempTermVector->tfidf;
					break;
				}
				tempTermVector = tempTermVector->next;
			}
		}
		tempDocVector->score = score;
		tempDocVector = tempDocVector->next;
	}
}


void sortDocVector(){
	
	docVector *tempDocVectorOutside;
	docVector *tempDocVectorInside;

	int tempDocId;
	float tempScore;
	termVector *temp;

	tempDocVectorOutside = head;

	while(tempDocVectorOutside != NULL){
		tempDocVectorInside = tempDocVectorOutside->next;

		while(tempDocVectorInside != NULL){
			if(tempDocVectorOutside->score < tempDocVectorInside->score){
				tempDocId = tempDocVectorOutside->docId;
				tempDocVectorOutside->docId = tempDocVectorInside->docId;
				tempDocVectorInside->docId = tempDocId;
				
				tempScore = tempDocVectorOutside->score;
				tempDocVectorOutside->score = tempDocVectorInside->score;
				tempDocVectorInside->score = tempScore;

				temp= tempDocVectorOutside->termVectorPtrHead;
				tempDocVectorOutside->termVectorPtrHead = tempDocVectorInside->termVectorPtrHead;
				tempDocVectorInside->termVectorPtrHead = temp;

				temp = tempDocVectorOutside->termVectorPtrEnd;
				tempDocVectorOutside->termVectorPtrEnd = tempDocVectorInside->termVectorPtrEnd;
				tempDocVectorInside->termVectorPtrEnd = temp;
				
			}
			tempDocVectorInside = tempDocVectorInside->next;
		}
			
		tempDocVectorOutside = tempDocVectorOutside->next;
	}
}

void printDocVector() {
	docVector *temp;
	temp = head;

	while(temp != NULL) {
		printf("%s %lf\n",docs[temp->docId].name,temp->score);
		temp = temp->next;
	}
}

void writeDocVectorToFile() {
	docVector *temp;
	temp = head;

	FILE *fp;

	fp = fopen("SearchResults.txt","w");
	while(temp != NULL) {
		fprintf(fp,"%s\n",docs[temp->docId].name);
		temp = temp->next;
	}
	fclose(fp);
}
