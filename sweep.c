#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <unistd.h>
#include "util.h"

int main (int argc, char **argv)
{
  double freqf;
  int usecs;
  double lowfreq, hifreq;

  pruinit(&argc, argv);

  if (argc != 4) {
    printf ("Usage: %s <lowfreq> <hifreq> <usecs>\n  Where <usecs> is the number of microseconds to delay between frequency changes\n", argv[0]);
    exit(0);
    }

  lowfreq = atof(argv[1]);
  hifreq = atof(argv[2]);
  usecs = atoi(argv[3]);

  while (1) {
    for (freqf=lowfreq; freqf<hifreq; freqf++) {
      setfreq(freqf);
      usleep(usecs);
      }
    }

  return(0);
}



