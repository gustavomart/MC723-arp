#include <stdio.h>
#include <math.h>

#define GLOBAL_LOCK 0x500000
#define AcquireGlobalLock while(*g_lock)
#define ReleaseGlobalLock *g_lock=0
volatile int *g_lock = (int*) GLOBAL_LOCK;
volatile int lock1 = 1;

void inline AcquireLocalLock(int *lock)
{
  AcquireGlobalLock;
  while(*lock)
  {
    ReleaseGlobalLock;
    AcquireGlobalLock;
  };
  ReleaseGlobalLock;
}

void inline ReleaseLocalLock(int *lock)
{
  AcquireGlobalLock;
  *lock=0;
  ReleaseGlobalLock;
}

int main(int argc, char *argv[]) 
{ 
  int i = 0;
  
  for (i=0; i < 10000; i++)
  {
    AcquireLocalLock(&lock1);
    printf("Lock: %d\n", lock1); 
    ReleaseLocalLock(&lock1);
  }

  exit(0); // To avoid cross-compiler exit routine
  return 0; // Never executed, just for compatibility
}

