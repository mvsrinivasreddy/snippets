#include<stdio.h>

union{
	int	data;
	struct{
		unsigned char	w1;
		unsigned char	w2;
		unsigned char	w3;
		unsigned char	w4;
	}word;
}tu = {0x321};

int main(){

	extern int i;
	i=10;
	printf("%d\n", i);
	printf(" byte1: %x byte2: %x byte3: %x byte4: %x\n", tu.word.w1, tu.word.w2, tu.word.w3, tu.word.w4);
}	
