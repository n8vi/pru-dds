#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"
#include "dds_bin.h"

#define IRAM (PRUNUM+2)

int main (void)
{
  pruinit();

  prussdrv_pru_write_memory(IRAM, 0, (unsigned int *)PRUcode, sizeof(PRUcode));

  return(0);
}

