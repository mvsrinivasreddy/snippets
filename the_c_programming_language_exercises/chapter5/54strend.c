#include<stdio.h>
#include<string.h>

int strend(char *s, char *t){
	int slen = strlen(s);
	int tlen = strlen(t);
	printf("slen:%d, tlen:%d\n", slen,tlen);
	if(tlen > slen)
		return 0;
	else{
		int ind = slen-tlen; 
		for(int i=0; i<tlen; i++,ind++){
			//if(s[ind++] == *t++)
			if(*(s+ind) == *t++)
				continue;
			else
				return 0;
		}
	}	
	return 1;
}



int main(){
	char	str[100], ttr[100];
	char *s=str, *t=ttr;
	int i=0;
	
	while((s[i++] = (char)getchar()) != '\n'){
	}
	s[i] = '\0';	
	printf("Got string S: %s\n", s);
	
	i=0;
	while((t[i++] = (char)getchar()) != '\n'){}
	t[i] = '\0';	
	printf("Got string T: %s\n", t);

	if(strend(s,t) == 1)
		printf("Occurs\n");
	else
		printf("does not Occur\n");
}
