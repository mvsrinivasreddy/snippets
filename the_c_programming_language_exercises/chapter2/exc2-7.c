#include <stdio.h>

int invert(int x , int p, int n){
	
	int t, nv, m;
	
	t  = (~(~0 << n)) << (p-n);
	nv = (~(t & x)) & t; // n bits at position inverted and remaining 0
	m = ~t & x; // make n bits at position p 0 in x
	
	//return m | nv;
	return x^t;
}

int main(){
	
	printf("Invert: %d\n", invert(5026, 8, 3));
	return 0;
}
