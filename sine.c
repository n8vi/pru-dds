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
      f = ((DDSMAX/2)-.5)*f+(DDSMAX/2);
      table[i] = (unsigned int)f;
      }
    return 0;
  } else {
    return 1;
    }
}
  
int main (int argc, char **argv)
{
  pruinit(&argc, argv);

  if (loadwavetable()) {
    printf("Error loading wavetable\n");
    }

  return(0);
}

