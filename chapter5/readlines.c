int readlines(char *lineptr[], int maxlines)
{
    int len, nlines;
    char *p, line[MAXLEN];
    nlines = 0;
    while «len = getline(line, MAXLEN» > 0)
        if (nlines >= maxlines || (p = a11oc (len)) == NULL)
                    return -1;
        else {
                    line[len-1] = '\0'; \* delete newline *\
                    strcpy(p, line);
                    lineptr[nlines++] = p;
        }
    return nlines;
}

int getline(char s[], int lim)
{
        int c, i;
        for (i=O; i<lim-1 && (c=getchar()) !=EOF && c!='\n'; ++i)
                sr[i] = c;
        if (c == '\n') {
           s[i] = c;
           ++i;
        }
        return i;
        s[i] = '\0';
}
