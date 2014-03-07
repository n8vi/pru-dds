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

static int _prunum;

unsigned char *wavetable()
{
  unsigned char *pruDataMem = (unsigned char *)0;

  //Initialize pointer to PRU data memory
  if (_prunum == 0) {
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void**)&pruDataMem);
  } else if (_prunum == 1) {
    prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, (void**)&pruDataMem);
    }

  return pruDataMem;
}

void wavetable2signed()
{
  int i, j;
  char s;
  unsigned char u;
  unsigned char *utable;
  char *stable;

  utable = wavetable();
  stable = (char *)utable;

  for(j=0; j<TABLELEN; j++) {
    u = utable[j];
    i = u;
    i -= 128;
    s = i;
    stable[j] = s;
    }
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

  /* Store int in DDR memory location */
  DDR_regaddr1 = ddrMem + OFFSET_DDR + offset;

  *(unsigned long*) DDR_regaddr1 = ddrint;

  // Reset PRU instruction pointer to get it to reload value from DDR
  // perhaps there's a better way, but this works for me
  // prussdrv_pru_reset() blows away whatever code is in the PRU, and we don't want that.

  prussdrv_pru_disable ( _prunum );
  prussdrv_pru_enable ( _prunum );

  return(0);
}

int setddrchar (int offset, int ddrchar)
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

  /* Store char in DDR memory location */
  DDR_regaddr1 = ddrMem + OFFSET_DDR + offset;

  *(char *) DDR_regaddr1 = ddrchar;

  // Reset PRU instruction pointer to get it to reload value from DDR
  // perhaps there's a better way, but this works for me
  // prussdrv_pru_reset() blows away whatever code is in the PRU, and we don't want that.

  prussdrv_pru_disable ( _prunum );
  prussdrv_pru_enable ( _prunum );

  return(0);
}

int setdramchar (int offset, char dramchar)
{
  char *dramMem;

  dramMem = (char *)(wavetable()+4096+offset);
  *dramMem = dramchar;

  return 0;
}

int setdramint (int offset, int dramint)
{
  int *dramMem;

  dramMem = (int *)(wavetable()+4096+offset);
  *dramMem = dramint;

  return 0;
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

unsigned long getddrchar (int offset)
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

  ret = *(char*) DDR_regaddr1;
  close(mem_fd);
  return ret;
}

int getdramchar (int offset)
{
  char *dramMem;

  dramMem = (char *)(wavetable()+4096+offset);
  return *dramMem;
}

int getdramint (int offset)
{
  int *dramMem;

  dramMem = (int *)(wavetable()+4096+offset);
  return *dramMem;
}

int setamp(char amp)
{
  #ifdef REALTIMEFREQ
    return(setdramchar(5, amp));
  #else
    return(setddrchar(5, amp));
  #endif
}

int getamp(char amp)
{
  #ifdef REALTIMEFREQ
    return(getdramchar(5));
  #else
    return(getddrchar(5));
  #endif
}

int setskip (double skiplenf)
{
  int skiplen;

  if (skiplenf > (TABLELEN/2)) {
    return -1;
    }

  skiplenf *= TABLEMULT;  // <<=19
  skiplen = (int)skiplenf;

#ifdef REALTIMEFREQ
  return(setdramint(0, skiplen));
#else
  return(setddrint(0, skiplen));
#endif
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
#ifdef REALTIMEFREQ
  ret = (double)getdramint(0);
#else
  ret = (double)getddrint(0);
#endif
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

int getprunum(int *argc, char **argv)
{
  int prunum;
  char *prustr = NULL;
  int i,j;

  for (i=0; i<*argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == 'p') {
      if (argv[i][2] == '\0') {
        prustr = argv[i+1];
        for(j=i+2; j<*argc; j++) {
          argv[j-2] = argv[j];
          }
        *argc -= 2;
      } else {
        prustr = argv[i]+2;
        for(j=i+1; j<*argc; j++) {
          argv[j-1] = argv[j];
          }
        *argc -= 1;
        }
      }
    }

  if (! prustr) {
    return PRUNUM;
    }
  prunum = atoi(prustr);
  if (prunum != 0 && prunum != 1) {
    printf("ERROR: Invalid PRU specified (%d)\n", prunum);
    exit(0);
    }
  return prunum;
}

int prunum()
{
  return _prunum;
}

int pruinit (int *argc, char **argv)
{
  int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  _prunum = getprunum(argc, argv);

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret) {
    printf("prussdrv_open open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  return(0);
}

