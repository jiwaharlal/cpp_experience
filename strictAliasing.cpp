#include <stdio.h>
#include <cstdint>
#include <stdint.h>

typedef unsigned int DWORD;
typedef unsigned short WORD;

long foo(int *x, long *y) {
  *x = 0;
  *y = 1;
  return *x;
}

inline DWORD SwapWords ( DWORD val )
{
   WORD * p = (WORD*) &val;
   WORD t;
   t = p[0];
   p[0] = p[1];
   p[1] = t;
   return val;
}

inline uint64_t GetIt ( const DWORD * p )
{
   return *(uint64_t*)p;
}

int main()
{
   long l;
   printf("%ld\n", foo((int *)&l, &l));

   printf ( "%d\n", SwapWords(1) );

   DWORD buf[10];
   uint64_t t;

   buf[0] = 1;
   buf[1] = 2;
   t = GetIt(buf);
   buf[2] = 3;
   buf[3] = 4;

   printf ( "%d, %d, %d, %d\n", buf[0], buf[1], int(t>>32), int(t) );

   return 0;
}


