/* htoi - convert hexa to integer */
#include <stdio.h>
#include <string.h>


int power(int base, int n){
	
	int result=base;
	if(n == 0)
	  return 1;
		
	for(; n>1; n--)
		result = result*base;	
		
	return result;
}

int main(int argc,char* argv[])
{
    int counter, ind, base, result=0, pos=0;
    char arg[7];
    printf("Program Name Is: %s",argv[0]);
    if(argc==1)
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
    if(argc>=2)
    {
        printf("\nNumber Of Arguments Passed: %d",argc);
        printf("\n----Following Are The Command Line Arguments Passed----");
        for(counter=1;counter<argc;counter++){
            printf("\nargv[%d]: %s\n",counter,argv[counter]);
	    strcpy(arg, argv[counter]); 
            printf("\narg: %s\n",arg);
	    ind = strlen(arg) - 1 ;
	    	//for (; ind>=0; ind--, pos++){
	    	for (ind=0; ind<6; ind++){
				if(arg[ind] == '\0')
					break;
				if((arg[ind] >= '0') && (arg[ind] <= '9'))
					base = arg[ind] - '0';
				else if((arg[ind] >= 'A') && (arg[ind] <= 'F'))
					base = 10 + (arg[ind] - 'A');	
				else if((arg[ind] >= 'a') && (arg[ind] <= 'f'))
					base = 10 + (arg[ind] - 'a');
				else{
					printf("Invalid input:[%c]!\n", arg[ind]);
					break;
				}
			
				//result = result+ (base * power(16, pos)); 
				result = result * 16 +  base ; 
				printf("i Loop Result: %d\n", result);
			}
			printf("Result: %d\n", result);
		}
	    
    }
    
    return 0;
}
