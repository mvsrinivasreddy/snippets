/*
 * funcptr.c
 *
 *  Created on: 02-Apr-2019
 *      Author: srinivas
 */

#include <stdio.h>
#include <string.h>
#define MAXLINES 5000
#define MAXLEN 1000

char *lineptr[MAXLINES];// max #lines to be sorted *1 1* pointers to text lines *1
int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort(void *lineptr[], int left, int right,int (*comp) (void *, void *));
int numcmp(char *, char *); //1* sort input lines *1

int getline(char s[], int lim)
{
	int c, i;
	for (i=0; i<lim-1 && (c=getchar()) !=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	return i;
	s[i] = '\0';
}


//1* readlines: read input lines *1
int readlines(char *lineptr[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];
	nlines = 0;
	while (len = getline(line, MAXLEN) > 0)
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else{
			line[len-1] = '\0'; //1* delete newline *1
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	return nlines;
}
//1* writelines: write output lines *1
void writelines(char *lineptr[], int nlines)
{
	int i;
	for (i = 0; i < nlines; i++)
		printf("%s\n", lineptr[i]);
}

void swap(void *v[], int i, int j)
{
	void *temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

//1* numcmp: compare s1 and s2 numerically *1
int numcmp(char *s1, char *s2)
{
	double v1, v2;
	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	else
		return 0;
}


//1* qsort: sort v[left] •..v[right] into increasing order *1
void qsort(void *v[], int left, int right,int (*comp)(void *, void *))
{
	int i, last;
	void swap(void *v[], int, int);
	if (left >= right) //1* do nothing if array contains *1
		return; //1* fewer than two elements *1
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((comp)(v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	qsort(v, left, last-1, comp);
	qsort(v, last+1, right, comp);
}

main(int argc, char *argv[])
{
	int nlines;
	int numeric = 0; //1* number of input lines read *11* 1 if numeric sort *1

	if (argc > 1 && strcmp(argv[1], "-n") == 0)
		numeric = 1;
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort((void **) lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric ? numcmp : strcmp));
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("input too big to sort\n");
		return 1;
	}
}
