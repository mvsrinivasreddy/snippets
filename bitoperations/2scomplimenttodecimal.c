#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mpow(int x, int y){
    while(y>0){
        x += x*x;
        y--;
    }
    return x;
}

int main(){
    
    int data=0;
    printf("enter 2s compliment:");
    scanf("%d", &data);

   // data = 59136;
    printf("number = %x\n",data);
    data = data>>7;     //for specific LM75 temp sensor reading
    //check if negative number
    if(data & (1<<8)){
        printf("its a negative number:%x\n",data) ;
        data = data & ~(1<<8);     //clear sign bit
        printf(" sign clear number = %x\n",data);
        data = ~data;              //inverse all bits
        printf(" ~ number = %x\n",data);
        data += 1;
        printf(" + number = %x\n",data);
        data &= 0xFF;
        data *= -1;
        printf("number = %d\n",data);
        printf("temperature = %d C\n",data/2);  //1 unit if Temp is represesnted as 0.5 degrees
    }
    else{
        data &= ~(1<<8);
        printf("its a positive number:%d\n",data);
        printf("temperature = +%d C\n",data/2);  //1 unit if Temp is represesnted as 0.5 degrees
    }

    return 0;
}
