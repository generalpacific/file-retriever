/*
	read.h

	Header file for read.c
*/

#ifndef _read_
#define _read 1

#include"header.h"

int readWord(FILE *fp, char *c);
int readLine(FILE *fp, char *c);

// Following functions used for reading index

int readTerm(FILE *fp, char *c);
void skipToNewLine(FILE *fp);
int getDf(FILE *fp);
void getRecord(FILE *fp, char *c);

#endif
