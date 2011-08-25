
#include"stopwords.h"

char stopWord[26][MAXIMUM][MAXIMUM];
void initializeStopWordsArray() {

	char file[MAXIMUM];
	char alpha[2];
	char temp[MAXSMALL];

	FILE *fp;

//	printf("In initializeStopWordsArray\n");

	strcpy(file,"stopwords/");

	alpha[1] = '\0';
	int i;
	for(i = 97; i <= 122; ++i) {
		alpha[0] = i;
		strcat(file,alpha);
			
//		printf("file = %s\n",file);
		
		fp = fopen(file,"r");
		
		int j = 0;
		while(readLine(fp,temp) != FAIL) {
//			printf("%s:%d\n",temp,j);
			strcpy(stopWord[i-97][j++],temp);
		}
		strcpy(stopWord[i-97][j],"XXXX");
		strcpy(file,"stopwords/");

		fclose(fp);

	}


}

int checkForStopWords(char *word) {

	if(word[0] < 97 || word[0] > 122)
		return FAIL;

	int i = word[0] - 97;
	int j = 0;

//	printf("In Check stop words i = %d\n",i);
	while(strcmp(stopWord[i][j],"XXXX") != 0) {

		if(strcmp(stopWord[i][j],word) == 0)
			return SUCCESS;
		j++;
	}

	return FAIL;
}
