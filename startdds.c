// #include <stdio.h>
// #include <sys/mman.h>
// #include <fcntl.h>
// #include <errno.h>
// #include <unistd.h>
// #include <string.h>
// #include <math.h>
// #include <stdlib.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

// #define PRU_NUM          0

// #define DDR_BASEADDR     0x80000000
// #define OFFSET_DDR       0x00001000

#define PI (3.141592653585)

int main (void)
{
  pruinit();

  prussdrv_exec_program (PRU_NUM, "./dds.bin");

  return(0);
}

