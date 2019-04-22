#include<stdio.h>
#include<time.h>
#include<unistd.h>

int main(){
int Ind;

for(Ind=0;; Ind++){
	printf("INd:%d\n",(Ind%10));
    usleep(500000);
}
}
