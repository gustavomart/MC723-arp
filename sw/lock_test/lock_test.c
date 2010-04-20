#include <stdio.h>
#include <math.h>

#define ENDERECO_LOCK 0x500000
#define CreateLock volatile int *lock = (int*) ENDERECO_LOCK
#define AcquireLock while(*lock);
#define ReleaseLock *lock=0;

int main(int argc, char *argv[]){
 
  int i = 0;
  CreateLock;
  
  for (i=0; i < 10000; i++)
  {
    printf("Lock: %d\n", *lock);

    *lock = i;

    printf("Lock: %d\n", *lock); 
  }

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

