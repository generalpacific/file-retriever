
#include"processwords.h"

void convertToLowerCase(char *word) {
	int i = 0;

	while(word[i] != '\0') {
		if(word[i] >= 65 && word[i] <=90) {
			word[i] = word[i] + 32;
		}
		i++;
	}
}

void removeSpecialSymbols(char *word) {

	if(word[0] >= 48 && word[0]<= 57) {
		return;
	}

	int i = 0;

	char temp[MAXIMUM];

	int j = 0;

	while(word[i] != '\0'){
		if((word[i] >= 97 && word[i] <=122) || (word[i] >= 48 && word[i] <=57) || word[i] == ' ') {
			temp[j++] = word[i++];
			continue;
		}
		i++;
	}
	temp[j] = '\0';

	strcpy(word,temp);

}
