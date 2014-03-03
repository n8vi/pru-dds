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
        table[i] = (DDSMAX-1);
      } else {
        table[i] = 0;
        }
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

