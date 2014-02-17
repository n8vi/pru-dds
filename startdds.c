#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM          0

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000

#define PI (3.141592653585)

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

  system("./sine");
  system("./setfreq 10000");

  prussdrv_pruintc_init(&pruss_intc_initdata);

  prussdrv_exec_program (PRU_NUM, "./dds.bin");

  return(0);
}

