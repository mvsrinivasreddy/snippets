#include<stdio.h>
#include<unistd.h>

int main(){
int addr= 128;
int size=300;
int psize=128;
int btow=0, br=0, eop=0;

int div=addr/psize;
printf("div=%d\n",div);
int poaddr = div+1;
br = size;

while(1){
	sleep(1);
	eop = poaddr*psize;
	btow = eop-addr; //no of bytes that can be written in this page
	if(br > btow){
		printf("WRITE %d bytes FROM %d addr till %d eop pageNo:%d\n", btow, addr,eop, poaddr);	
		addr = addr+btow; // update addr location
		poaddr+=1; // move to next page
		br = br - btow;
	}
	else{
		printf("WRITE %d bytes FROM %d addr, done writing\n", br, addr);
		break;	
	}
}


return 0;
}
