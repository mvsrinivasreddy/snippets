#include<stdio.h>

int main(){
	int n=1234, count=0;
	
	while(n>0){
		n=n&(n-1);
		count++;
	}
	printf("count:%d\n", count);
}
