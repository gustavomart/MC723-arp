#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
  int i;
  int a, b, c;

  a = 34;
  b = 38;
  c = 12;

  for (i = 0; i < 10000; i++) {
  	a = b + c;
    c = a - b;
    printf("%d %d %d\n", a, b, c);
    a = a - b - c;
    c = c - a + b;
  }

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

