#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main (void)
{
  pruinit();

  prussdrv_exec_program (PRU_NUM, "./dds.bin");

  return(0);
}

