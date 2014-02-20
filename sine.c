#include <stdio.h>
#include <math.h>
#include "util.h"

#define PI (3.141592653585)

static int loadwavetable()
{
  int i;
  double f;
  unsigned char *table;

  table = wavetable();
  if (table) {
    for (i=0; i<TABLELEN; i++) {
      f = sin(2*PI*i/TABLELEN);
      f = 127.5*f+128;
      table[i] = (unsigned int)f;
      }
    return 0;
  } else {
    return 1;
    }
}
  
int main (void)
{
  pruinit();

  if (loadwavetable()) {
    printf("Error loading wavetable\n");
    }

  return(0);
}

