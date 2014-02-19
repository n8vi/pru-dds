#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "config.h"

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM          0

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000

#define PI (3.141592653585)

static int loadwavetable(unsigned short pruNum)
{
  int i;
  double f;
  unsigned char *pruDataMem;

  //Initialize pointer to PRU data memory
  if (pruNum == 0)
  { 
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void**)&pruDataMem);
  } else if (pruNum == 1) { 
    prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, (void**)&pruDataMem);
    }

  for (i=0; i<TABLELEN; i++) {
    f = sin(2*PI*i/TABLELEN);
    f = 127.5*f+128;
    pruDataMem[i] = (unsigned int)f;
    }
  return(0);
}

  
int main (void)
{
  unsigned int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret) {
    printf("prussdrv_open open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  // you can stop and restart the PRU if you want, but I think this looks neat

  loadwavetable(PRU_NUM);

  return(0);
}

