

#include"read.h"

int readTerm(FILE *fp, char *c){
	int i = 0;
	char ch;

	if(fp == NULL )
		return FAIL;
	
	while(1){
		ch = fgetc(fp);

		if(ch == EOF)
			return FAIL;

		if(ch == '$'){
			break;
		}

		c[i++] = ch;
	}
	c[i] = '\0';
	return SUCCESS;
}

void skipToNewLine(FILE *fp){
	char ch;

	while(1){
		ch = fgetc(fp);
		if(ch == '\n' || ch == EOF)
			return;
	}
}

int getDf(FILE *fp){
	char ch;
	char temp[30];

	int i = 0;

	while(1){
		ch = fgetc(fp);
		if(ch == '$'){
			break;
		}
		temp[i++] = ch;
	}
	temp[i] = '\0';

	i = atoi(temp);

	return i;
}
void getRecord(FILE *fp, char *c){
	char ch;
	int i = 0;

	while(1){
		ch = fgetc(fp);
		if(ch == '#'){
			break;
		}
		c[i++] = ch;
	}

	c[i] = '\0';

	
}

int readWord(FILE *fp, char *retWord) {

	int i = 0;

	char ch;

	if(fp == NULL)
		return FAIL;

	while(1) {
		ch = fgetc(fp);
		if(ch == EOF)
			return FAIL;

		if(ch == ' ' || ch == '\n' || ch == '\t')
			break;
		retWord[i++] = ch;
		if(i == MAXIMUM){
			break;
		}

	}	

	retWord[i] = '\0';
	return SUCCESS;
}


int readLine(FILE *fp, char *retLine) {

	int i = 0;

	char ch;

//	printf("in getline\n");

	while(1) {
		ch = fgetc(fp);
		if(ch == EOF){
			return FAIL;
		}
		if(ch == '\n')
			break;
		retLine[i++] = ch;
		if(i == MAXBIG){
			break;
		}

	}
	
	retLine[i] = '\0';

	return SUCCESS;
}

