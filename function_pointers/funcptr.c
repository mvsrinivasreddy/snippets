#include<stdio.h>

int callback(char   errcode)
{
    switch(errcode){
        case 1: printf("Error code 1: grave mistake done!!\n");
                return 1; break;
        case 2: printf("Error code 2: grave mistake resolved!!\n");
                return 2; break;
        case 3: printf("Error code 3: grave mistake redone!!\n");
                return 2; break;
               default: printf("failed option\n"); return -1;
    }
}

int testcb(char code, int (*cb)(char))
//int testcb(char code, (int (*)(char))(cb))
{
    return cb(code);
}

int main(){
     
    testcb(3, callback);   
    testcb(1, callback);   
    testcb(2, callback);   
    return 0;
}
