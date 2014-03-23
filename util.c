// Copyright (c) 2014 Brad J. Tarratt

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

static int _mypru;
static int _mainpru;

unsigned char *wavetable()
{
  unsigned char *pruDataMem = (unsigned char *)0;

  //Initialize pointer to PRU data memory
  if (_mainpru == 0) {
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void**)&pruDataMem);
  } else if (_mainpru == 1) {
    prussdrv_map_prumem (PRUSS0_PRU1_DATARAM, (void**)&pruDataMem);
    }

  return pruDataMem;
}

unsigned char *mydram()
{
  unsigned char *pruDataMem = (unsigned char *)0;

  //Initialize pointer to PRU data memory
  if (_mypru == 0) {
    prussdrv_map_prumem (PRUSS0_PRU0_DATARAM, (void**)&pruDataMem);
  } else if (_mypru == 1) {
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

  utable = (unsigned char *)wavetable();
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

  prussdrv_pru_disable ( _mypru );
  prussdrv_pru_enable ( _mypru );

  return(0);
}

int setddrchar (int offset, char ddrchar)
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

  prussdrv_pru_disable ( _mypru );
  prussdrv_pru_enable ( _mypru );

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

char getddrchar (int offset)
{
  int mem_fd;
  void *ddrMem;
  void *DDR_regaddr1;
  char ret;

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

int setamp(int amp)
{
  if (amp < -127 || amp > 127) {
    return -1;
     }
  #ifdef REALTIMEAMP
    // return(setdramchar(4, amp));
    return(setdramint(4, amp));
  #else
    // return(setddrchar(4, amp));
    return(setddrint(4, amp));
  #endif
}

int getamp()
{
  #ifdef REALTIMEAMP
    // return(getdramchar(4));
    return(getdramint(4));
  #else
    // return(getddrchar(4));
    return(getddrint(4));
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

void getprunum(int *argc, char **argv, int whichpru)
{
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
    _mainpru = MAINPRUNUM;
    if (whichpru) {
      _mypru = 1-_mainpru;
    } else {
      _mypru = _mainpru;
      }
    return;
    }
  _mypru = atoi(prustr);
  if (_mypru != 0 && _mypru != 1) {
    printf("ERROR: Invalid PRU specified (%d)\n", _mypru);
    exit(0);
    }
  if (whichpru) {
    _mainpru = 1-_mypru;
  } else {
    _mainpru = _mypru;
    }
    
}

int mypru()
{
  return _mypru;
}

int otherpru()
{
  return 1-_mypru;
}

int mainpru()
{
  return _mainpru;
}

int auxpru()
{
  return 1-_mainpru;
}

int pruinit (int *argc, char **argv, int whichpru)
{
  int ret;
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

  getprunum(argc, argv, whichpru);

  prussdrv_init ();

  ret = prussdrv_open(PRU_EVTOUT_0);
  if (ret) {
    printf("prussdrv_open(PRU_EVTOUT_0) open failed\n");
    return (ret);
    }

  ret = prussdrv_open(PRU_EVTOUT_1);
  if (ret) {
    printf("prussdrv_open(PRU_EVTOUT_1) open failed\n");
    return (ret);
    }

  prussdrv_pruintc_init(&pruss_intc_initdata);

  return(0);
}

