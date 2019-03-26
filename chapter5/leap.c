#include <stdio.h>

int main(){
	int leap, year =1981;
	
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	printf("is leap:%d\n", leap);

}
