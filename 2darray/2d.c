#include <stdio.h>
#include <stdlib.h>

int check(int* b){
  printf("%d\n", *((b+0)+6));
  return 0;
}

/*
* 
*/
int main() {
  int a[3][3] = {{5, 15, 10}, {34, 54, 11}, {14, 2, 13}};
 
  int *b = a;
  check(a[0]);
  //printf("%d\n", *(q-3));
  //printf("%d\n", *q-*p);
  //printf("%d\n", *p<*q);
  return 0;
}
