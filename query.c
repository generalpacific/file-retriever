
#include"query.h"


char operators[MAXSMALL][MAXSMALL];
char indexTermsInQuery[MAXIMUM][MAXIMUM];
int indexTermsInQueryCount;

void getIndexTerms(char * query) {
	int i = 0, j = 0, addingStarted = FAIL;

	char temp[MAXIMUM];
	

	while(query[i] != '\0') {
		if(query[i] == ' ' || query[i] == '\t') {
			temp[j] = '\0';
			j = 0;

			if(strcmp(temp,"and") != 0 || strcmp(temp,"or") != 0 || strcmp(temp,"not") != 0) {
				strcpy(indexTermsInQuery[indexTermsInQueryCount], temp);
				indexTermsInQueryCount++;
			}
			++i;
		}

		temp[j] = query[i];
		++i;
		++j;

	}

	temp[j]	= '\0';
	if(strcmp(temp,"and") != 0 || strcmp(temp,"or") != 0 || strcmp(temp,"not") != 0) {
		strcpy(indexTermsInQuery[indexTermsInQueryCount], temp);
		indexTermsInQueryCount++;
	}
}

void removeStopWordsFromQuery(char * query) {
	int i = 0, j = 0, addingStarted = FAIL;

	char temp[MAXIMUM],final[MAXIMUM];
	
	final[0] = '\0';

	while(query[i] != '\0') {
		if(query[i] == ' ' || query[i] == '\t') {
			temp[j] = '\0';
			j = 0;

			//printf("in remove stopwords, temp = %s\n",temp);

			if(strcmp(temp,"and") == 0 || strcmp(temp,"or") == 0 || strcmp(temp,"not") == 0) {
				if(addingStarted == SUCCESS) {
					strcat(final," ");
					strcat(final,temp);
				}
			}
			else {
				if(checkForStopWords(temp) == FAIL) {
					if(addingStarted == SUCCESS) {
						strcat(final," ");
						strcat(final,temp);
					}
					else {
						strcat(final,temp);
						addingStarted = SUCCESS;
					}
				}
			}
			++i;
		}

		temp[j] = query[i];
		++i;
		++j;

	}

	temp[j]	= '\0';
	if(checkForStopWords(temp) == FAIL) {
		if(addingStarted == SUCCESS) {
			strcat(final," ");
			strcat(final,temp);
		}
		else
			strcat(final,temp);
	}
	strcpy(query,final);
}

void evaluateQuery(char *query) {
	int i = 0, j = 0;
	char temp[MAXIMUM];
	int lastOperatorAdded = FAIL;
	int addStarted = FAIL;

//	printf("In processQuery %s\n",query);

	while(query[i] != '\0') {


		if(query[i] == ' ' || query[i] == '\t') {
//			printf("In processQuery, in if\n");
			temp[j] = '\0';
			j = 0;
			
			if(strcmp(temp,"and") == 0) {
//				printf("In processQuery, and\n");
				strcpy(operators[operatorCount],"and");
				operatorCount++;
				lastOperatorAdded = SUCCESS;
			}
			else if(strcmp(temp,"or") == 0) {
				strcpy(operators[operatorCount],"or");
				operatorCount++;
				lastOperatorAdded = SUCCESS;
			}
			else if(strcmp(temp,"not") == 0) {
				strcat(operators[operatorCount - 1]," not");
				lastOperatorAdded = SUCCESS;
			}
			else {

				searchTermUpdateStack(temp);
	//			printf("last operator added flag : %d %s\n",lastOperatorAdded,temp);

				if(lastOperatorAdded == FAIL && addStarted == SUCCESS){
	//				printf("In and number 2\n");
					strcpy(operators[operatorCount],"and");
					operatorCount++;
					lastOperatorAdded = FAIL;
				}
				else if(lastOperatorAdded == SUCCESS && addStarted == SUCCESS) {
					lastOperatorAdded = FAIL;
				}
				
				if(addStarted == FAIL) {
					addStarted = SUCCESS;
				}

			}
		} else {
			temp[j] = query[i];
			++j;
		}
		++i;
	}

	temp[j] = '\0';
	searchTermUpdateStack(temp);
	if(lastOperatorAdded == FAIL && addStarted == SUCCESS){
	//	printf("In and number 2\n");
		strcpy(operators[operatorCount],"and");
		operatorCount++;
		lastOperatorAdded = FAIL;
	}

	/* For testing purposes only */
/*	printf("\nTesting\n");
	for(i = 0; i < operatorCount; ++i) {
		printf("%s\n",operators[i]);
	}*/

	//printStack();

	i = operatorCount - 1;
	while(i >= 0) {
		Result *head1, *head2, *resultTemp;

		head1 = popResult();
		head2 = popResult();

		if(strcmp(operators[i],"and") == 0) {
			resultTemp = andOp(head1, head2);
		}
		else if(strcmp(operators[i],"or") == 0) {
			resultTemp = orOp(head1,head2);
		}
		else if(strcmp(operators[i],"and not") == 0) {
			resultTemp = andNotOp(head1,head2);
		}

		pushResult(resultTemp);
		--i;
	}
	//printStack();

}
