#ifndef _UTIL_H

#include "config.h"

#define PRU_NUM          0
#define DDR_BASEADDR     0x80000000
#define OFFSET_DDR       0x00001000

int setskip (double skiplenf);
unsigned char *wavetable();

int setfreq(double freq);
int pruinit (void);


#endif
