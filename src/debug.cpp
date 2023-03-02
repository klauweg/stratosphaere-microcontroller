#include <stdint.h>
#include <stdio.h>

void binprint(uint64_t old)
{
   char buffer[70];
   uint64_t input = old;
   printf("--------++++++++........~~~~~~~~--------++++++++........~~~~~~~~\n");
   for (int i=0; i<=63; i++)
   {
      buffer[i] = input & 1 ? '1' : '0';
      input >>= 1;
   }
   for (int i=63; i>=0; i--)
      printf("%c",buffer[i]);
   printf("\nunsigned: %lu    signed: %ld\n",old,old );
}

