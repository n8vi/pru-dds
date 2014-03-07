#ifndef _UTIL_H

#include "config.h"

#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000

unsigned char *wavetable();
int setfreq(double freq);
double getfreq();
int prunum();
int pruinit (int *argc, char **argv);
void wavetable2signed(void);

#endif
