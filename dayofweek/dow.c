#include<stdio.h>
#include<unistd.h>
 
int dayofweek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}
 
/* Driver function to test above function */
int main()
{
    int i=0, day = 1, Ind, offset;
    day += dayofweek(25, 10, 2017);
    printf ("\n%d\n", day);

    day = (1<<10)-1;
    printf ("\n%d\n", day);

 
  /*  for(i=7;i<=12;i=i%12){
	printf("MOd:%d\n",i);
	i++;
	sleep(1);
    }*/

 for(Ind=29, offset=1; (Ind<=32&&offset<=32); Ind=Ind%32,offset++){
printf("Ind:%d\n",Ind);
	
if(1073741824 &(1<<(Ind-1))){
printf("IN check\n");
}	
Ind++;
usleep(500);
}


    return 0;
}
