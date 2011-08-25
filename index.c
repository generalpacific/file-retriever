/*	Author 	: Chaudhary Prashant
	Desc 	: The main function creating the index by reading file names from the file named 'files.txt'
	License	: GPLv3
*/


#include"index.h"
#include"fileExtension.h"

extern int numOfDocs;
extern int fileLen[MAXDOCS];

int main(){
//int indexer() {

	FILE *files,*fp,*conf;
	char temp[MAXIMUM],tempFileName[MAXIMUM],filesName[MAXIMUM],textFileName[MAXIMUM];
	int i,docId = 1;
	int fileType;
	int ret;
	int totalErrors = 0;

	initializeStopWordsArray();
	initializeTrie();

/*	conf = fopen("CONFIG","r");
	readLine(conf,filesName);
	fclose(conf);*/

	files = fopen("INDEX/files.txt","r");

	if(files == NULL){
		goto createStatusFile;
	}
	
	while(readLine(files,tempFileName) != FAIL) {
		printf("%d \n",docId);
		printf("%s\n",tempFileName);

		fileType = checkFileExtension(tempFileName);

		strcpy(textFileName,"temp.txt");
		if(fileType == PDF){
			ret = convertPdfToTxt(tempFileName,textFileName);
			if(ret != SUCCESS){
				++totalErrors;
				++docId;
				continue;
			}
		}
		else if(fileType == DOC){
			ret = convertDocToTxt(tempFileName,textFileName);
			if(ret != SUCCESS){
				++totalErrors;
				++docId;
				continue;
			}
		}
		else if(fileType == ODT){
			ret = convertOdtToTxt(tempFileName,textFileName);
			if(ret != SUCCESS){
				++totalErrors;
				++docId;
				continue;
			}
		}else if(fileType == TXT){
			strcpy(textFileName,tempFileName);
		}else if(fileType == 0){
			++totalErrors;
			++docId;
			continue;
		}

		fp = fopen(textFileName,"r");

		if(fp == NULL){
				++totalErrors;
			++docId;
			continue;
		}

		// 	Read each filename from the file 'files.txt'
		//	For each filename, read each word and add to index trie.

	
		while(readWord(fp,temp) != FAIL) {
			
			convertToLowerCase(temp);
			removeSpecialSymbols(temp);

//			printf("%s ",temp);

			if(temp[0] == '\0' || temp[0] == '\t' || temp[0] == ' ' || temp[0] == '\n')
				continue;

			if(checkForStopWords(temp) != SUCCESS) {
				int i = 0;

				// Presently on the words containing letters are allowed 
				while(temp[i] != '\0') {
					if(temp[i] < 97 || temp[i] > 122)
						break;
					++i;
				}
				if(temp[i] == '\0') {
					fileLen[docId]++;
//					printf("%d %s\n",docId,temp);
					addTerm(temp,docId);
				}

				//if(temp[0] >= 97 && temp[0]<=122) {
				//	addTerm(temp,docId);
				//}
				//printf("%s\n",temp);
			}
		}

		++docId;

		fclose(fp);
		if(fileType != TXT){
			system("rm temp.txt");
			system("touch temp.txt");
		}
		printf("...Done\n");
	}


	//printf("\nTrie Traversal\n");
	//traverseTerms();
	numOfDocs = docId - 1;

	printf("Number of docs : %d\n",numOfDocs);
	printf("Number of Errors : %d\n",totalErrors);

//	traverseTerms();
	createIndexFile();
	
	createStatusFile:
	system("echo \"Index Status\n==================\nIndex created on :\n\"  > INDEX/STATUS.txt");
	system("date >> INDEX/STATUS.txt");
	system("echo \"Directory indexed : \"$HOME >> INDEX/STATUS.txt");
	sprintf(temp,"echo \"No of Documents indexed : %d\" >> INDEX/STATUS.txt",numOfDocs);
	system(temp);

	return 0;
}


