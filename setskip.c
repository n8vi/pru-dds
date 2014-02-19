#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#include "util.h"

int main (int argc, char **argv)
{
  unsigned int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  double skiplenf;

  if (argc != 2) {
    printf("Usage %s <skiplen> %d\n", argv[0], argc);
    return 0;
    }

  skiplenf = atof(argv[1]);

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret) {
    printf("prussdrv_open open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  setskip(skiplenf);

  // Reset PRU instruction pointer to get it to reload skiplen from DDR
  // perhaps there's a better way, but this works for me
  // prussdrv_pru_reset() blows away whatever code is in the PRU, and we don't want that.

  prussdrv_pru_disable ( 0 );
  prussdrv_pru_enable ( 0 );

  return(0);
}

