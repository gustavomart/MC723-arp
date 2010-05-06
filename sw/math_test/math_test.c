#include <stdio.h>
#include <math.h>

#define A 90

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

  int i, j, k;
  double matriz[A][A];
  double result[A][A];

  for (i=0; i < A; i++)
    for (j=0; j < A; j++)
      matriz[i][j] = i+j;

  for (i = 0; i < A; i++)
  {
    for (j = 0; j < A; j++)
    {
      result[i][j] = 0;
      for (k = 0; k < A; k++)
      {
        result[i][j] = add(result[i][j], mult(matriz[i][k], matriz[k][j]));
      }
    }
  }

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

