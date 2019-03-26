#include <stdio.h>

int rightrot(unsigned char x, unsigned char n){
	return ((~(~0 << n) & x) << n) | (x >> n);
}

int main(){
	
	printf("right: %d\n", rightrot(248, 4));	

	return 0;
}
