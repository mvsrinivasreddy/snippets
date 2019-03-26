#include<stdio.h>

int setbits(int x, int p, int n, int y){
			
	int extx, exty, extp;
	
	exty = ~(~0 << n) & y;		// extracts last n bits of y clean
	printf("exty:%d\n",exty);
	extp = 	~((~(~0 << n) << (p-n))) ;		// prepares flag to set n bits from position p to 0		
	printf("extp:%d\n",extp);
	extx = 	extp & x;		// sets n bits from position p to 0		
	printf("extx:%d\n",extx);

	return  extx | (exty << (p-n));
}


int main(){	

	printf("x: %d", setbits(3917, 8, 3, 254));
	return 0;

}
