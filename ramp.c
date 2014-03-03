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
      f = DDSMAX-1;
      f = TABLELEN/f;
      f = i/f;
      table[i] = (unsigned char)f;
// i / (TABLELEN/(DDSMAX-1)) ;
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

