#include<stdio.h>
#include<string.h> 
#include<stdlib.h> 
int main() 
{ 
char trans1[12]; 
char *trans; 
strncpy(trans1,"bdd32ef7",12); 
printf("\nIn str[%s]\n",trans1); 
printf("\nIn hex[%x]\n",(unsigned int)strtol(trans1,NULL,16)); 
//printf("\ntrans[%x]\n",(unsigned int)trans); 
printf("\nIn long[%d]\n",(unsigned int)strtol(trans1,NULL,16)); 
}
