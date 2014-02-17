#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM          0

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000

static int setskip (int skip)
{
  int mem_fd;
  void *ddrMem;
  void *DDR_regaddr1;

  /* open the device */
  mem_fd = open("/dev/mem", O_RDWR);
  if (mem_fd < 0) {
    printf("Failed to open /dev/mem (%s)\n", strerror(errno));
    return -1;
    }

  /* map the DDR memory */
  ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
  if (ddrMem == NULL) {
    printf("Failed to map the device (%s)\n", strerror(errno));
    close(mem_fd);
    return -1;
    }

  /* Store Addends in DDR memory location */
  DDR_regaddr1 = ddrMem + OFFSET_DDR;
  
  *(unsigned long*) DDR_regaddr1 = skip;

  return(0);
}

int main (int argc, char **argv)
{
  unsigned int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  double skiplenf;
  int skiplen;

  if (argc != 2) {
    printf("Usage %s <frequency> %d\n", argv[0], argc);
    return 0;
    }

  skiplenf = atof(argv[1]);

  if (skiplenf > 4096.0) {
    printf("Skiplen %f exceeds 1/2 table length.  Nyquist says no.\n", skiplenf);
    return(0);
    }

  skiplenf *= 524288;  // <<=19
  skiplen = (int)skiplenf;

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret) {
    printf("prussdrv_open open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  setskip((int)skiplen);

  // Reset PRU instruction pointer to get it to reload skiplen from DDR
  // perhaps there's a better way, but this works for me
  // prussdrv_pru_reset() blows away whatever code is in the PRU, and we don't want that.

  prussdrv_pru_disable ( 0 );
  prussdrv_pru_enable ( 0 );

  return(0);
}

