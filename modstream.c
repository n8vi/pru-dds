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

int main (int argc, char **argv)
{
  int f, f0, f1;
  int i;
  int retval;
  unsigned char *buf;
  fd_set rfds;
  struct timeval tv;

  pruinit(&argc, argv, AUXPRU);

  f0 = prussdrv_pru_event_fd(PRU_EVTOUT_0);
  f1 = prussdrv_pru_event_fd(PRU_EVTOUT_1);

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <somefile.raw>\n", argv[0]);
    return 0;
    }

  f = open(argv[1], O_RDONLY);

  if (f == -1) {
    fprintf(stderr, "Cannot open %s\n", argv[1]);
    return 0;
    }

  buf = mydram();

  // read(f, buf, 8192);

  prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
  prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);

  while (1) {

    FD_ZERO(&rfds);
    FD_SET(f0, &rfds);
    FD_SET(f1, &rfds);
 
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    retval = select(f1+1, &rfds, NULL, NULL, &tv);

    if (retval == -1) {
      perror("select()");
    } else if (retval && FD_ISSET(f0, &rfds)) {
      if (!read(f, buf, 4096)) {
        return 0;
        }
      read(f0, &i, 4);
      // printf("PRU_EVTOUT_0 %d\n", i);
      prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
    } else if (retval && FD_ISSET(f1, &rfds)) {
      if (!read(f, buf+4096, 4096)) {
        return 0;
        }
      read(f1, &i, 4);
      // printf("PRU_EVTOUT_1 %d\n", i);
      prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
    } else {
      printf("No interrupts within five seconds (start modulator first!).\n");
      exit(0);
      }
    }

  return(0);
}

