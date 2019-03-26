#include <stdio.h>
#include <string.h>

#define LINE_FOLD	10
#define LINE_MAX	15

int main(){
	char	Result[1000]="";
	char	LineBuf[200];
	int 	BlP=0;
	int	c, ind, i, ResInd=0;
	
	for(ind=0; ((ind < 200 ) && ((c = getchar()) != EOF)); ind++){
		if(c == '\n'){		
			printf("hit line fold!\n");
			for(i=0; i<ind; i++, BlP++){
				
				if(LineBuf[i] != '\0')
					Result[ResInd++] = LineBuf[i];
				else
					printf("received 0 at %d ie.,[%s]\n", i, &LineBuf[i-5]);
				
				if((LineBuf[i] == ' ') && BlP > LINE_FOLD){
					Result[ResInd++] = '\n';
					BlP = 0;
				}
			}
			printf("hit line fold logic end\n");
			ind=0;	//reset LineBuf index	
			memset(LineBuf, 0, sizeof(LineBuf));
			printf("loop Result:[%s]\n", Result);
			ResInd -= 2;
		}
		else	
			LineBuf[ind] = c;
			
	}
	printf("Result:[%s]\n", Result);
	return 0;
}
