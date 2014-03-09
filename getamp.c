#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main (int argc, char **argv)
{
#ifndef AMPLITUDE
  fprintf(stderr, "$amplitude option not configured at compile time, no amplitude capability\n");
  exit(0);
#endif
  pruinit(&argc, argv);

  printf("Current amplitude: %d\n", getamp());

  return(0);
}

