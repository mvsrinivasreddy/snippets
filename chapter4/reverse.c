#include<stdio.h>
#include<string.h>

int m;
int n;

void reverse(char* s){
    char temp;
    printf("rev1:%s\n",s);
    temp = s[m];
    s[m] = s[n];
    s[n] = temp;
    n--;m++;
    if((n-m)<=1)
        return;
    printf("rev:%s\n",s);
    reverse(s);
}

int main(int argc, char* argv[]){
    m=0; 
    n=strlen(argv[1])-1;
    reverse(argv[1]);
    printf("str:%s\n",argv[1]);
    return 0;
}
