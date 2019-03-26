#include <stdio.h>

#define TABSTOP 	8

int main(){
	int 	in, i=0, tabcnt=0;
	char	output[2000];
	
	while((in = getchar()) != EOF){
		if(in == '\t'){
			for(tabcnt=0; tabcnt<TABSTOP; tabcnt++, i++){
				output[i] = ' ';
			}
		}
		else{
			output[i] = in;
			i++;
		}
	}
	output[++i] = '\0';
	printf("putput:[%s]\n", output);

	return 0;
}

