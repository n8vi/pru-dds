#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main (int argc, char **argv)
{
  double freqf;
  int ret;

  if (argc != 2) {
    printf("Usage %s <frequency>\n", argv[0]);
    return 0;
    }

  freqf = atof(argv[1]);

  pruinit();

  ret = setfreq(freqf);

  if (ret) {
    printf("Error setting freq %f\n", freqf);
    return(0);
    }

  return(0);
}

