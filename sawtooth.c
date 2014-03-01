#include <stdio.h>
#include "util.h"

static int loadwavetable()
{
  int i;
  double f;
  unsigned char *table;

  table = wavetable();

  if (table) {
    for (i=0; i<TABLELEN; i++) {
      if (i&(TABLELEN/2)) {
        f = DDSMAX - 1;
        f = TABLELEN/f;
        f = (2*i)/f;
        f -= DDSMAX;
        f = DDSMAX - 1 - f;
        table[i] = (unsigned char)f;
        // table[i] = DDSMAX - 1 - ((2* i / (TABLELEN/(DDSMAX-1)))-DDSMAX);
      } else {
        f = DDSMAX - 1;
        f = TABLELEN/f;
        f = (2*i)/f;
        table[i] = (unsigned char)f;
        // table[i] = 2* i / (TABLELEN/(DDSMAX-1)) ;
        }
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

