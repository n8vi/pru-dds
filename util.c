#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#include "util.h"
#include "config.h"

unsigned char *wavetable()
{
  unsigned char *pruDataMem = (unsigned char *)0;

  //Initialize pointer to PRU data memory
#if PRUNUM == 0
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void**)&pruDataMem);
#elif PRUNUM == 1
    prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, (void**)&pruDataMem);
#endif

  return pruDataMem;
}

int setddrint (int offset, int ddrint)
{
  int mem_fd;
  void *ddrMem;
  void *DDR_regaddr1;

  /* open the device */
  mem_fd = open("/dev/mem", O_RDWR);
  if (mem_fd < 0) {
    return -1;
    }

  /* map the DDR memory */
  ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
  if (ddrMem == NULL) {
    close(mem_fd);
    return -1;
    }

  /* Store skiplen in DDR memory location */
  DDR_regaddr1 = ddrMem + OFFSET_DDR + offset;

  *(unsigned long*) DDR_regaddr1 = ddrint;

  // Reset PRU instruction pointer to get it to reload value from DDR
  // perhaps there's a better way, but this works for me
  // prussdrv_pru_reset() blows away whatever code is in the PRU, and we don't want that.

  prussdrv_pru_disable ( PRUNUM );
  prussdrv_pru_enable ( PRUNUM );

  return(0);
}

unsigned long getddrint (int offset)
{
  int mem_fd;
  void *ddrMem;
  void *DDR_regaddr1;
  int ret;

  /* open the device */
  mem_fd = open("/dev/mem", O_RDWR);
  if (mem_fd < 0) {
    return -1;
    }

  /* map the DDR memory */
  ddrMem = mmap(0, 0x0FFFFFFF, PROT_WRITE | PROT_READ, MAP_SHARED, mem_fd, DDR_BASEADDR);
  if (ddrMem == NULL) {
    close(mem_fd);
    return -1;
    }

  DDR_regaddr1 = ddrMem + OFFSET_DDR + offset;

  ret = *(unsigned long*) DDR_regaddr1;
  close(mem_fd);
  return ret;
}

int setskip (double skiplenf)
{
  int skiplen;

  if (skiplenf > (TABLELEN/2)) {
    return -1;
    }

  skiplenf *= TABLEMULT;  // <<=19
  skiplen = (int)skiplenf;

  return(setddrint(0, skiplen));
}

int setfreq(double freq)
{
  double skiplen;
  skiplen = (freq * TABLELEN * CPUCYCLES) / CLOCKRATE;
  return setskip(skiplen);
}

double getskip()
{
  double ret;
  ret = (double)getddrint(0);
  ret /= TABLEMULT;
  return ret;
}

double getfreq()
{
  double ret;

  ret = getskip() * CLOCKRATE;
  ret /= (TABLELEN*CPUCYCLES);
  ret *= 10;
  ret = round(ret);
  ret /= 10;

  return ret;
}

int pruinit (void)
{
  int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);  // is this specific per PRU?
  if (ret) {
    printf("prussdrv_open open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  return(0);
}

