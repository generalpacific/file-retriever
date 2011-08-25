
#include"header.h"
#include"fileExtension.h"

int checkFileExtension(char *filename){
	int i,l;
	char ext[5];

	l = strlen(filename);

	ext[0] = filename[l - 4];
	ext[1] = filename[l - 3];
	ext[2] = filename[l - 2];
	ext[3] = filename[l - 1];
	ext[4] = '\0';

	if(strcmp(ext,".pdf")==0) {
		return PDF;
	}else if(strcmp(ext,".doc")==0){
		return DOC;
	}else if(strcmp(ext,".txt")==0){
		return TXT;
	}else if(strcmp(ext,".odt")==0){
		return ODT;
	}

	return 0;
}

int convertDocToTxt(char *docFile,char *txtFile) {
	char cmd[MAXIMUM+MAXIMUM];
	int ret;

	sprintf(cmd,"antiword -i 1 -t \"%s\" > \"%s\"",docFile,txtFile);

	ret = system(cmd);
	if(ret == 0)
		return SUCCESS;
}

int convertOdtToTxt(char *docFile,char *txtFile) {
	char cmd[MAXIMUM+MAXIMUM];
	int ret;

	sprintf(cmd,"unoconv --stdout \"%s\" > \"%s\"",docFile,txtFile);

	ret = system(cmd);
	if(ret == 0)
		return SUCCESS;
}

int convertPdfToTxt(char *docFile,char *txtFile) {
	char cmd[MAXIMUM+MAXIMUM];
	int ret = 0;
	int pid, status;

	char *arg[] = {"pdftolatex","-eol","unix","-f","1","-l","10",docFile,txtFile,NULL};
	char *newEnv[] = {NULL};


	sprintf(cmd,"pdftotext -eol unix -q -f 1 -l 10 \"%s\" \"%s\"",docFile,txtFile);

	pid = fork();
	if(pid < 0){
		return FAIL;
	}
	if(pid == 0){
		ret = execve("/usr/bin/pdftotext",arg,newEnv);
		perror("execve");
		exit(0);
	}else{
		wait(&status);
	}

//	ret = system(cmd);
	if(ret == 0)
		return SUCCESS;
}
