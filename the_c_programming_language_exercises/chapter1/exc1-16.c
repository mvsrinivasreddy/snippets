#include <stdio.h>
#define MAXLINE 1000 /* maximum input line size */
#define LONGLINE_LIM 15
int getaline(char s[], int lim);
void copy(char to[], char from[]);
/* print longest input line */
int main()
{
int len;
int max;
char line[MAXLINE];
char longest[MAXLINE];
max = 0;
while ((len = getaline(line, MAXLINE)) >0)
	if (len> LONGLINE_LIM){
		max = len;	
		copy(longest, line);	
		printf("Length:[%d] : %s,", max, longest);
	}
	if (max> 0) /* there was a line */
		printf("Length:[%d] : %s,", max, longest);
	return 0;
}
/* getline: read a line into s, return length */
int getaline(char s[], int lim)
{
int c, i;
for (i=0; i<lim-1 && (c=getchar()) !=EOF && c != '\n'; ++i)
	s[i] = c;
if (c == '\n') {
	s[i] = c;
	++i;
}
s[i] = '\0';
return i;
}
/* copy: copy 'from' into 'to'; assume to is big enough */
void copy(char to[], char from[])
{
int i;
i=0;
while ((to[i] = from[i]) != '\0'){
	++i;
}
}
