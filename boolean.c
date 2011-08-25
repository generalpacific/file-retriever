
#include"boolean.h"

Result* andOp(Result *head1, Result *head2) {
	Result *tempResult1, *tempResult2, *finalResult, *finalResultEnd;

	tempResult1 = head1;
	tempResult2 = head2;
	finalResult = NULL;
	finalResultEnd = NULL;

	while(tempResult1 != NULL && tempResult2 != NULL) {
		
		if(tempResult1->docId == tempResult2->docId) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->docId = tempResult1->docId;
				finalResult->next = NULL;
				finalResultEnd = finalResult;
			}
			else {
				Result *newResultNode;
				newResultNode = (Result*)malloc(sizeof(Result));
				newResultNode->docId = tempResult1->docId;
				newResultNode->next = NULL;

				finalResultEnd->next = newResultNode;
				finalResultEnd = newResultNode;
			}

			tempResult1 = tempResult1->next;
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId > tempResult2->docId) {
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId < tempResult2->docId) {
			tempResult1 = tempResult1->next;
		}
	}

	return finalResult;
}

/*old and op for unsorted postings*/
/*Result* andOp(Result *head1, Result *head2) {
	Result *tempResult1, *tempResult2, *finalResult, *finalResultEnd;

	tempResult1 = head1;
	finalResult = NULL;
	finalResultEnd = NULL;

	while(tempResult1 != NULL) {
		int docId = tempResult1->docId;

		tempResult2 = head2;
		int found = FALSE;

//		printf("in and Op , docId1 = %d\n",docId);

		while(tempResult2 != NULL) {
//			printf("\tin and Op , docId2 = %d\n",tempResult2->docId);
			if(tempResult2->docId == docId) {
				found = TRUE;
				break;
			}
			tempResult2 = tempResult2->next;
		}

		if(found == TRUE) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->next = NULL;
				finalResult->docId = docId;
				finalResultEnd = finalResult;
			}
			else {
				Result *tempResult;
				tempResult = (Result*)malloc(sizeof(Result));
				tempResult->next = NULL;
				tempResult->docId = docId;

				finalResultEnd->next = tempResult;
				finalResultEnd = tempResult;
			}
		}

		tempResult1 = tempResult1->next;
		
	}

	return finalResult;
}*/

Result* orOp(Result *head1, Result *head2) {
	Result *tempResult1, *tempResult2, *finalResult, *finalResultEnd;

	tempResult1 = head1;
	tempResult2 = head2;
	finalResult = NULL;
	finalResultEnd = NULL;

	while(tempResult1 != NULL && tempResult2 != NULL) {
		
		if(tempResult1->docId == tempResult2->docId) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->docId = tempResult1->docId;
				finalResult->next = NULL;
				finalResultEnd = finalResult;
			}
			else {
				Result *newResultNode;
				newResultNode = (Result*)malloc(sizeof(Result));
				newResultNode->docId = tempResult1->docId;
				newResultNode->next = NULL;

				finalResultEnd->next = newResultNode;
				finalResultEnd = newResultNode;
			}
			tempResult1 = tempResult1->next;
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId > tempResult2->docId) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->docId = tempResult2->docId;
				finalResult->next = NULL;
				finalResultEnd = finalResult;
			}
			else {
				Result *newResultNode;
				newResultNode = (Result*)malloc(sizeof(Result));
				newResultNode->docId = tempResult2->docId;
				newResultNode->next = NULL;

				finalResultEnd->next = newResultNode;
				finalResultEnd = newResultNode;
			}
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId < tempResult2->docId) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->docId = tempResult1->docId;
				finalResult->next = NULL;
				finalResultEnd = finalResult;
			}
			else {
				Result *newResultNode;
				newResultNode = (Result*)malloc(sizeof(Result));
				newResultNode->docId = tempResult1->docId;
				newResultNode->next = NULL;

				finalResultEnd->next = newResultNode;
				finalResultEnd = newResultNode;
			}
			tempResult1 = tempResult1->next;
		}
	}

	while(tempResult1 != NULL) {
		if(finalResult == NULL) {
			finalResult = (Result*)malloc(sizeof(Result));
			finalResult->docId = tempResult1->docId;
			finalResult->next = NULL;
			finalResultEnd = finalResult;
		}
		else {
			Result *newResultNode;
			newResultNode = (Result*)malloc(sizeof(Result));
			newResultNode->docId = tempResult1->docId;
			newResultNode->next = NULL;

			finalResultEnd->next = newResultNode;
			finalResultEnd = newResultNode;
		}
			tempResult1 = tempResult1->next;
	}

	while(tempResult2 != NULL) {
		if(finalResult == NULL) {
			finalResult = (Result*)malloc(sizeof(Result));
			finalResult->docId = tempResult2->docId;
			finalResult->next = NULL;
			finalResultEnd = finalResult;
		}
		else {
			Result *newResultNode;
			newResultNode = (Result*)malloc(sizeof(Result));
			newResultNode->docId = tempResult2->docId;
			newResultNode->next = NULL;

			finalResultEnd->next = newResultNode;
			finalResultEnd = newResultNode;
		}
			tempResult2 = tempResult2->next;
	}
	return finalResult;
}

Result* andNotOp(Result *head1, Result *head2) {
	Result *tempResult1, *tempResult2, *finalResult, *finalResultEnd;

	tempResult1 = head1;
	tempResult2 = head2;
	finalResult = NULL;
	finalResultEnd = NULL;

	while(tempResult1 != NULL && tempResult2 != NULL) {
		
		if(tempResult1->docId == tempResult2->docId) {
			tempResult1 = tempResult1->next;
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId > tempResult2->docId) {
			if(finalResult == NULL) {
				finalResult = (Result*)malloc(sizeof(Result));
				finalResult->docId = tempResult2->docId;
				finalResult->next = NULL;
				finalResultEnd = finalResult;
			}
			else {
				Result *newResultNode;
				newResultNode = (Result*)malloc(sizeof(Result));
				newResultNode->docId = tempResult2->docId;
				newResultNode->next = NULL;

				finalResultEnd->next = newResultNode;
				finalResultEnd = newResultNode;
			}
			tempResult2 = tempResult2->next;
		}
		else if(tempResult1->docId < tempResult2->docId) {
			tempResult1 = tempResult1->next;
		}
	}


	while(tempResult2 != NULL) {
		if(finalResult == NULL) {
			finalResult = (Result*)malloc(sizeof(Result));
			finalResult->docId = tempResult2->docId;
			finalResult->next = NULL;
			finalResultEnd = finalResult;
		}
		else {
			Result *newResultNode;
			newResultNode = (Result*)malloc(sizeof(Result));
			newResultNode->docId = tempResult2->docId;
			newResultNode->next = NULL;

			finalResultEnd->next = newResultNode;
			finalResultEnd = newResultNode;
		}
			tempResult2 = tempResult2->next;
	}
	return finalResult;
}
