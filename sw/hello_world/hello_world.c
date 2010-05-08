#include <stdio.h>

int main(int argc, char *argv[]){
  int i;
  int a[1024], sum=0;

  for (i=0; i < 1024; i++)
    a[i] = i;

  for(i=0;i<10000;i++)
    printf("Hi from processor MIPS!\n");

  for (i=0; i< 1024; i++)
    sum+=a[i];

  printf("Soma = %d\n", sum);  
  
  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

