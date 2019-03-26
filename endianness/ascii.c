#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main(){
	unsigned char arr[6] = {48, 49, 50, 51,52,53};
	int data = atoi((const char*)arr);
	//int data = atoi(arr);
	printf("%d\n", data);	
	scanf("%d", data);	
}
