#include <stdio.h>
#include <string.h>
char* lower(char *str, char *res){
	
	for(int i=0; str[i] != '\0'; i++){
		res[i] = (str[i] >= 'A' && str[i] <= 'Z') ? ('a' + (str[i] - 'A')) : str[i];
	}
	
	return res;
	
}


int main(){
	
	char	Result[64] = "";
	
	printf("Lower Case: %s\n", lower("HELLo WoRLd123", Result));
	return 0;
}
