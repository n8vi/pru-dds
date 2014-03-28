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
#include <termios.h>
#include <signal.h>
#include <math.h>

#define PI (3.141592653585)
#define BAUD (31.25)

static double modsamprate = 8000;
static double symsiz;

static struct termios bak;

unsigned short varicode[128] =	{
0b1010101011000000, // NUL
0b1011011011000000, // SOH
0b1011101101000000, // STX
0b1101110111000000, // ETX
0b1011101011000000, // EOT
0b1101011111000000, // ENQ
0b1011101111000000, // ACK
0b1011111101000000, // BEL
0b1011111111000000, // BS
0b1110111100000000, // HT
0b1110100000000000, // LF
0b1101101111000000, // VT
0b1011011101000000, // FF
0b1111100000000000, // CR
0b1101110101000000, // SO
0b1110101011000000, // SI
0b1011110111000000, // DLE
0b1011110101000000, // DC1
0b1110101101000000, // DC2
0b1110101111000000, // DC3
0b1101011011000000, // DC4
0b1101101011000000, // NAK
0b1101101101000000, // SYN
0b1101010111000000, // ETB
0b1101111011000000, // CAN
0b1101111101000000, // EM
0b1110110111000000, // SUB
0b1101010101000000, // ESC
0b1101011101000000, // FS
0b1110111011000000, // GS
0b1011111011000000, // RS
0b1101111111000000, // US
0b1000000000000000, // SP
0b1111111110000000, // !
0b1010111110000000, // "
0b1111101010000000, // #
0b1110110110000000, // $
0b1011010101000000, // %
0b1010111011000000, // &
0b1011111110000000, // '
0b1111101100000000, // (
0b1111011100000000, // )
0b1011011110000000, // *
0b1110111110000000, // +
0b1110101000000000, // ,
0b1101010000000000, // -
0b1010111000000000, // .
0b1101011110000000, // /
0b1011011100000000, // 0
0b1011110100000000, // 1
0b1110110100000000, // 2
0b1111111100000000, // 3
0b1011101110000000, // 4
0b1010110110000000, // 5
0b1011010110000000, // 6
0b1101011010000000, // 7
0b1101010110000000, // 8
0b1101101110000000, // 9
0b1111010100000000, // :
0b1101111010000000, // ;
0b1111011010000000, // <
0b1010101000000000, // =
0b1110101110000000, // >
0b1010101111000000, // ?
0b1010111101000000, // @
0b1111101000000000, // A
0b1110101100000000, // B
0b1010110100000000, // C
0b1011010100000000, // D
0b1110111000000000, // E
0b1101101100000000, // F
0b1111110100000000, // G
0b1010101010000000, // H
0b1111111000000000, // I
0b1111111010000000, // J
0b1011111010000000, // K
0b1101011100000000, // L
0b1011101100000000, // M
0b1101110100000000, // N
0b1010101100000000, // O
0b1101010100000000, // P
0b1110111010000000, // Q
0b1010111100000000, // R
0b1101111000000000, // S
0b1101101000000000, // T
0b1010101110000000, // U
0b1101101010000000, // V
0b1010111010000000, // W
0b1011101010000000, // X
0b1011110110000000, // Y
0b1010101101000000, // Z
0b1111101110000000, // [
0b1111011110000000, // [BACKSLASH]
0b1111110110000000, // ]
0b1010111111000000, // ^
0b1011011010000000, // _
0b1011011111000000, // `
0b1011000000000000, // a
0b1011111000000000, // b
0b1011110000000000, // c
0b1011010000000000, // d
0b1100000000000000, // e
0b1111010000000000, // f
0b1011011000000000, // g
0b1010110000000000, // h
0b1101000000000000, // i
0b1111010110000000, // j
0b1011111100000000, // k
0b1101100000000000, // l
0b1110110000000000, // m
0b1111000000000000, // n
0b1110000000000000, // o
0b1111110000000000, // p
0b1101111110000000, // q
0b1010100000000000, // r
0b1011100000000000, // s
0b1010000000000000, // t
0b1101110000000000, // u
0b1111011000000000, // v
0b1101011000000000, // w
0b1101111100000000, // x
0b1011101000000000, // y
0b1110101010000000, // z
0b1010110111000000, // {
0b1101110110000000, // |
0b1010110101000000, // }
0b1011010111000000, // ~
0b1110110101000000, // DEL
};

static unsigned char *dram;

void chartty(void)
{
  struct termios t;

  tcgetattr(0, &t);
  memcpy(&bak, &t, sizeof(struct termios));

  t.c_lflag &= (~ICANON);
  t.c_cc[VTIME] = 0;
  t.c_cc[VMIN] = 1;
  t.c_lflag &= (~ECHO);
  tcsetattr(0, TCSANOW, &t);
}

void linetty(void)
{
  tcsetattr(0, TCSANOW, &bak);
  putchar('\n');
}

void newsighandler(int sigIn)
{
  linetty();

  exit(0);
}

// data needs to by a chunk of 8192 bytes, and the caller needs to rewind it each time we pass
// the 4096 mark (after doing something with the buffer, of course!)

void bit2psk(int t, unsigned char *data, int *pos)
{
  static unsigned int p=0;
  int i;
  double baseband;
  int samp;
  
  for (i=*pos; i<*pos+(symsiz*2); i++) {
    baseband = (cos((p*(PI)*BAUD/2)/modsamprate));
    baseband *= 128.0;
    samp = (int)baseband;
    samp += 128;
    data[i] = (unsigned char)samp;
    if (t == 0) {
      p++;
      }
    }
  *pos = i;
}

void char2psk(char c, unsigned char *data, int *pos)
{
  int t0=0, t1, i;

  for (i=15; i>=0; i--) {
    t1 = t0;
    t0 = ((varicode[c])>>i)&1;
    bit2psk(t0, data, pos);
    if ((t0|t1) == 0) {
      return;
      }
    }
}

void flushbuf(unsigned char *data, int *pos)
{
  memcpy(data, data+4096, 4096);
  *pos -= 4096;
  if (*pos < 0) {
    *pos = 0;
    }
}

void zeropad(unsigned char *data, int *pos)
{
  while (*pos < 4096) {
    bit2psk(0, data, pos);
    }
}

int main (int argc, char **argv)
{
  fd_set rfds;
  struct timeval tv;
  int retval;
  char c;
  int f0, f1, i;
  struct sigaction new_action, old_action;
  int pos=0;
  unsigned char buf[65536];

  symsiz = modsamprate/BAUD;

  pruinit(&argc, argv, AUXPRU);

  dram = mydram();

  prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
  prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
  f0 = prussdrv_pru_event_fd(PRU_EVTOUT_0);
  f1 = prussdrv_pru_event_fd(PRU_EVTOUT_1);

/*
  if (isatty(0)) {
    printf("I'm on a terminal\n");
  } else {
    printf("I'm NOT on a terminal\n");
    }
*/
     
  new_action.sa_handler = newsighandler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;
     
  sigaction (SIGINT, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGINT, &new_action, NULL);
  sigaction (SIGHUP, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGHUP, &new_action, NULL);
  sigaction (SIGTERM, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGTERM, &new_action, NULL);

  chartty();

  while (1) {

    FD_ZERO(&rfds);
    if (pos < 4096) {
      FD_SET(0, &rfds);
      }
    FD_SET(f0, &rfds);
    FD_SET(f1, &rfds);
  
    tv.tv_sec = 5;
    tv.tv_usec = 0;
  
    retval = select(f1+1, &rfds, NULL, NULL, &tv);

    if (retval == -1) {
      perror("select()");
    } else if (retval && FD_ISSET(0, &rfds)) {
      read(0, &c, 1);
      char2psk(c, buf, &pos);
      printf("%c", c);
    } else if (retval && FD_ISSET(f0, &rfds)) {
      read(f0, &i, 4);
      zeropad(buf, &pos);
      memcpy(dram, buf, 4096);
      flushbuf(buf, &pos);
      prussdrv_pru_clear_event (PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
    } else if (retval && FD_ISSET(f1, &rfds)) {
      read(f1, &i, 4);
      zeropad(buf, &pos);
      memcpy(dram+4096, buf, 4096);
      flushbuf(buf, &pos);
      prussdrv_pru_clear_event (PRU_EVTOUT_1, PRU1_ARM_INTERRUPT);
    } else {
      printf("No data within five seconds.\n");
      exit(0);
      }

     fflush(stdout);
    }

  return(0);
}

