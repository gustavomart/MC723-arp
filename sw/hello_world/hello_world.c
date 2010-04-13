#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
  int i;
  int a, b, c;

  a = 34;
  b = 38;
  c = 12;

  for (i = 0; i < 1000; i++) {
  	b = a*c;
  	c = a/c;
  	a = a+b;
  	b = a+c;
}

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

