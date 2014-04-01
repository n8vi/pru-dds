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

#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "util.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define MARKER (6400)
#define ZERO   (1600)
#define ONE    (4000)

typedef struct instant {
   time_t second;
   unsigned short sampno;
} instant;

short timecode(time_t sec)
{
  struct tm *t;
  int s;

  t = gmtime(&sec);
  s = sec % 60;

  switch (s) {
    case 0: return MARKER;
    case 1: return ((t->tm_min / 10)&4)?ONE:ZERO;
    case 2: return ((t->tm_min / 10)&2)?ONE:ZERO;
    case 3: return ((t->tm_min / 10)&1)?ONE:ZERO;
    case 4: return ZERO;
    case 5: return ((t->tm_min % 10)&8)?ONE:ZERO;
    case 6: return ((t->tm_min % 10)&4)?ONE:ZERO;
    case 7: return ((t->tm_min % 10)&2)?ONE:ZERO;
    case 8: return ((t->tm_min % 10)&1)?ONE:ZERO;
    case 9: return MARKER;
    case 10: return ZERO;
    case 11: return ZERO;
    case 12: return ((t->tm_hour / 10)&2)?ONE:ZERO;
    case 13: return ((t->tm_hour / 10)&1)?ONE:ZERO;
    case 14: return ZERO;
    case 15: return ((t->tm_hour % 10)&8)?ONE:ZERO;
    case 16: return ((t->tm_hour % 10)&4)?ONE:ZERO;
    case 17: return ((t->tm_hour % 10)&2)?ONE:ZERO;
    case 18: return ((t->tm_hour % 10)&1)?ONE:ZERO;
    case 19: return MARKER;
    case 20: return ZERO;
    case 21: return ZERO;
    case 22: return (((t->tm_yday+1) / 100)&2)?ONE:ZERO;
    case 23: return (((t->tm_yday+1) / 100)&1)?ONE:ZERO;
    case 24: return ZERO;
    case 25: return ((((t->tm_yday+1) / 10)%10)&8)?ONE:ZERO;
    case 26: return ((((t->tm_yday+1) / 10)%10)&4)?ONE:ZERO;
    case 27: return ((((t->tm_yday+1) / 10)%10)&2)?ONE:ZERO;
    case 28: return ((((t->tm_yday+1) / 10)%10)&1)?ONE:ZERO;
    case 29: return MARKER;
    case 30: return (((t->tm_yday+1)%10)&8)?ONE:ZERO;
    case 31: return (((t->tm_yday+1)%10)&4)?ONE:ZERO;
    case 32: return (((t->tm_yday+1)%10)&2)?ONE:ZERO;
    case 33: return (((t->tm_yday+1)%10)&1)?ONE:ZERO;
    case 34: return ZERO;
    case 35: return ZERO;
    case 36: return ZERO;	// + DUT1
    case 37: return ONE;	// - 
    case 38: return ZERO;	// +
    case 39: return MARKER;
    case 40: return ZERO;	// .8
    case 41: return ZERO;	// .4
    case 42: return ONE;	// .2
    case 43: return ZERO;	// .1
    case 44: return ZERO;
    case 45: return (((t->tm_year / 10)%10) & 8)?ONE:ZERO;
    case 46: return (((t->tm_year / 10)%10) & 4)?ONE:ZERO;
    case 47: return (((t->tm_year / 10)%10) & 2)?ONE:ZERO;
    case 48: return (((t->tm_year / 10)%10) & 1)?ONE:ZERO;
    case 49: return MARKER;
    case 50: return ((t->tm_year%10) & 8)?ONE:ZERO;
    case 51: return ((t->tm_year%10) & 4)?ONE:ZERO;
    case 52: return ((t->tm_year%10) & 2)?ONE:ZERO;
    case 53: return ((t->tm_year%10) & 1)?ONE:ZERO;
    case 54: return ZERO;
    case 55: return (t->tm_year+1900 % 4 == 0 && (t->tm_year+1900 % 100 != 0 || t->tm_year+1900 % 400 == 0))?ONE:ZERO;
    case 56: return ZERO;
    case 57: return (localtime(&sec)->tm_isdst > 0)?ONE:ZERO;
    case 58: return (localtime(&sec)->tm_isdst > 0)?ONE:ZERO; // you don't get warning.  Wah.
    case 59: return MARKER;
    }
  return 8000;
}

unsigned char *gensamples(instant *i)
{
  static unsigned char buf[8000*2];

  memset(buf, 5, timecode(i->second));
  memset(buf+timecode(i->second), 127, 8000-timecode(i->second));
  memset(buf+8000, 5, timecode(i->second+1));
  memset(buf+8000+timecode(i->second+1), 127, 8000-timecode(i->second+1));

  return buf+i->sampno;
}

int main (int argc, char **argv)
{
  int f0, f1;
  int i;
  int retval;
  unsigned char *buf;
  fd_set rfds;
  struct timeval tv;
  unsigned char *samps; // = fiveminofsamples();
  struct timeval tv2;
  struct timezone tz;
  instant inst;

  pruinit(&argc, argv, AUXPRU);

  f0 = prussdrv_pru_event_fd(PRU_EVTOUT_0);
  f1 = prussdrv_pru_event_fd(PRU_EVTOUT_1);

  buf = mydram();

  prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
  prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);

  while (1) {

    FD_ZERO(&rfds);
    FD_SET(f0, &rfds);
    FD_SET(f1, &rfds);
 
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(f1+1, &rfds, NULL, NULL, &tv);

    gettimeofday(&tv2, &tz);
    inst.second = tv2.tv_sec;
    inst.sampno = ((tv2.tv_usec*8)/1000)%8000;
    inst.sampno += 4096;
    inst.second += inst.sampno/8000;
    inst.sampno %= 8000;

    samps = gensamples(&inst);

    if (retval == -1) {
      perror("select()");
    } else if (retval && FD_ISSET(f0, &rfds)) {
      memcpy(buf, samps, 4096);
      samps += 4096;
      read(f0, &i, 4);
      prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
    } else if (retval && FD_ISSET(f1, &rfds)) {
      memcpy(buf+4096, samps, 4096);
      samps += 4096;
      read(f1, &i, 4);
      prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
    } else {
      printf("No interrupts within five seconds (start modulator first!).\n");
      exit(0);
      }
    }

  return(0);
}

