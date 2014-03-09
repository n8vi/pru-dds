#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main (int argc, char **argv)
{
  int amp;
  int ret;

#ifndef AMPLITUDE
  fprintf(stderr, "$amplitude option not configured at compile time, no amplitude capability\n");
  exit(0);
#endif

  pruinit(&argc, argv);

  if (argc != 2) {
    printf("Usage %s <amplitude>\n", argv[0]);
    return 0;
    }

  amp = atoi(argv[1]);

  ret = setamp(amp);

  if (ret) {
    printf("Error setting amplitude %d\n", amp);
    return(0);
    }

  return(0);
}

