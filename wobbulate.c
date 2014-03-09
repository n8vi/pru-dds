#include <stdio.h>
#include <math.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main(int argc, char **argv)
{
  double pi = 3.141592653585;
  double f;
  double ampf;
  int amp;

#ifndef AMPLITUDE
  fprintf(stderr, "$amplitude option not configured at compile time, no amplitude capability\n");
  exit(0);
#endif

  pruinit(&argc, argv);

  while(1) {
    for (f=0; f<2*pi; f+=.0005) {
      ampf = sin(f)*127;
      amp = (int) ampf;
      setamp(amp);
      }
    }
}
