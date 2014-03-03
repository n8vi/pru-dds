#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"
#include "dds_bin.h"

#define IRAM (prunum()+2)

int main (int argc, char **argv)
{
  pruinit(&argc, argv);

  prussdrv_pru_disable ( prunum() );
  prussdrv_pru_write_memory(IRAM, 0, (unsigned int *)PRUcode, sizeof(PRUcode));
  prussdrv_pru_enable ( prunum() );

  return(0);
}

