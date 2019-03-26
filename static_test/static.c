#include<stdio.h>
static int sVar = 44;

void myPrint2()
{
    printf("sVar = %d\n", sVar++);
}

void myPrint()
{
    static int sVar =88;
    printf("sVar = %d\n", sVar++);
}

int main(void)
{
    static int sVar = 55;
    int i = 0;
    for (i = 0; i < 5; i++)
        myPrint();      
    printf("sVar = %d\n", sVar);
    myPrint2();
    return(0);
}
