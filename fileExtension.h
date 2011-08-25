
/* defines functions for handling file extensions */

#ifndef _fileextension
#define _fileextension 1

#include"header.h"

#define TXT 2
#define DOC 4
#define PDF 8
#define ODT 16

int checkFileExtension(char *);
int convertDocToTxt(char *,char*);
int convertPdfToTxt(char*,char*);
int convertOdtToTxt(char*,char*);

#endif
