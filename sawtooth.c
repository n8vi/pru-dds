#include <stdio.h>
#include "util.h"

static int loadwavetable()
{
  int i;
  unsigned char *table;

  table = wavetable();

  if (table) {
    for (i=0; i<TABLELEN; i++) {
      if (i&(TABLELEN/2)) {
        table[i] = DDSMAX - 1 - ((2* i / (TABLELEN/(DDSMAX-1)))-DDSMAX);
      } else {
        table[i] = 2* i / (TABLELEN/(DDSMAX-1)) ;
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

