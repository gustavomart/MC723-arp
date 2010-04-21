#include <stdio.h>
#include <math.h>

#define REGM1 0x600000
#define REGM2 0x600008

#define REGA1 0x700000
#define REGA2 0x700008

#define RESM 0x600010
#define RESA 0x700010

double mult(double a, double b)
{
  double *r1=(double*)REGM1, *r2=(double*)REGM2, *res=(double*)RESM;

  *r1 = a;
  *r2 = b;
  
  return *res;
}

double add(double a, double b)
{
  double *r1=(double*)REGA1, *r2=(double*)REGA2, *res=(double*)RESA;

  *r1 = a;
  *r2 = b;
  
  return *res;
}

int main(int argc, char *argv[]) {
  int i;

  double b = 8.23;
  double c = 5.32;
  double d, e;



  
  for (i=0; i < 10000; i++) {
    //d = b*c;
    //e = b+c;
  d = mult(b,c);
  e = add(b,c);
  }

  printf ("d = %lf; e = %lf\n", d, e);

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

