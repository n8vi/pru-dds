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
#include "util.h"
#include "wobbulator_bin.h"

#define IRAM (otherpru()+2)

int main (int argc, char **argv)
{
  pruinit(&argc, argv);

  printf("starting wobbulator on PRU %d\n", otherpru());

  prussdrv_pru_disable ( otherpru() );
  prussdrv_pru_write_memory(IRAM, 0, (unsigned int *)PRUcode, sizeof(PRUcode));
  prussdrv_pru_enable ( otherpru() );

  return(0);
}

