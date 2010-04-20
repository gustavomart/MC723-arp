#include <stdio.h>
#include <math.h>

#define REG1_1 0x600000
#define REG1_2 0x600004
#define REG2_1 0x600008
#define REG2_2 0x60000C

#define RES_1 0x600010
#define RES_2 0x600014


int main(int argc, char *argv[]) {
  int i;
  //double a, b, c;

  //int *a = 0x400000;
  double b = 8.0;
  double c = 8.0;
  double d;

  int *r11=REG1_1, *r12=REG1_2, *r21=REG2_1, *r22=REG2_2;  
  int *res1=RES_1, *res2=RES_2;

  *r11 = *((int*)&b);
  *r12 = *(((int*)&b)+1);

  *r21 = *((int*)&b);
  *r22 = *(((int*)&b)+1);
    
  *((int*)&d) = *res1;
  *(((int*)&d)+1) = *res2;


  printf ("d = %lf\n", d);

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

