
/*
	Crawler for crawling the files in the current directory
*/


#include"header.h"

FILE *fp;

int checkForTextDoc(char *filename){
	int i,l;
	char ext[5];

	l = strlen(filename);

	ext[0] = filename[l - 4];
	ext[1] = filename[l - 3];
	ext[2] = filename[l - 2];
	ext[3] = filename[l - 1];
	ext[4] = '\0';

	if(strcmp(ext,".pdf")==0 || strcmp(ext,".doc")==0 || strcmp(ext,".txt")==0 || strcmp(ext,".odt") == 0) {
		return 1;
	}

	/*if(strcmp(ext,".txt")==0) 
		return 1;*/

	return 0;

}

void processDirectory(char *dirName) {
	
	DIR *in;
	struct dirent *dir;
	struct stat fileinfo;

	char filename[MAXBIG];

//	printf("In processDirectory dir = %s\n",dirName);

	in = opendir(dirName);
	while((dir = readdir(in)) != NULL) {
		filename[0] = '\0';
		strcat(filename,dirName);
		if(filename[strlen(filename) - 1] != '/') {
			strcat(filename,"/");
		}

		strcat(filename,dir->d_name);
		//printf("\tfilename = %s\n",filename);

		if(dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0 && dir->d_name[0] != '.') {
			processDirectory(filename);
		}
		else {
			stat(filename,&fileinfo);
			if((fileinfo.st_mode & S_IFMT) == S_IFREG && checkForTextDoc(filename)){
				printf("%s\n",filename);
				fprintf(fp,"%s\n",filename);
			}
		}
	}
	closedir(in);

}

int main(int argc, char **argv) {
	
	char dirName[MAXIMUM];
	struct stat fileinfo;

	printf("arg = %s\n",argv[1]);

	if(argc != 2) {
		printf("Usage : ./crawler <dir_name>\n");
		exit(0);
	}

	strcpy(dirName,argv[1]);

	if(stat(dirName,&fileinfo) == -1) {
		printf("ERROR : Invalid directory/file\n");
		sprintf(dirName,"touch INDEX/donecrawl");
		system(dirName);
		exit(0);
	}

	if((fileinfo.st_mode & S_IFMT) != S_IFDIR) { // check whether its a directory
		printf("ERROR : Argument not a directory\n");
		sprintf(dirName,"touch INDEX/donecrawl");
		system(dirName);
		exit(0);
	}
	else{
		// Process the directory
		fp = fopen("INDEX/files.txt","w");
		processDirectory(dirName);
		fclose(fp);

	}
	sprintf(dirName,"touch INDEX/donecrawl");
	system(dirName);

	return 0;
}

