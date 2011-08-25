

#include"stack.h"

stackNode *top;

void initializeStack() {
	top = NULL;
}

void pushResult(Result *head) {
	if(top == NULL) {
		top = (stackNode*)malloc(sizeof(stackNode));
		top->head = head;
		top->next = NULL;
		return;
	}

	stackNode *temp;
	temp = (stackNode*)malloc(sizeof(stackNode));
	temp->head = head;
	temp->next = top;
	top = temp;
	return;
}

Result* popResult() {
	if(top == NULL)
		return NULL;

	Result *returnNode;
	returnNode = top->head;
	
	stackNode *freeNode;
	freeNode = top;

	top = top->next;

	free(freeNode);
	return returnNode;
}

void printStack(){

	stackNode *tempStackNode;

	tempStackNode = top;

	printf("\n\nSTACK:\n");

	while(tempStackNode != NULL){

		printf("Next result : ");
		Result *tempResult;

		tempResult = tempStackNode->head;

		while(tempResult != NULL) {
			printf("%d -> ",tempResult->docId);
			tempResult = tempResult->next;
		}
	
		printf("\n");
		
		tempStackNode = tempStackNode->next;
	}
}
