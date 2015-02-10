// Copyright (c) 2014, 2015 Brad J. Tarratt

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
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include "util.h"

int main (int argc, char **argv)
{
  int amp;
  int ret;

#ifndef AMPLITUDE
  fprintf(stderr, "$amplitude option not configured at compile time, no amplitude capability\n");
  exit(0);
#endif

  pruinit(&argc, argv, MAINPRU);

  if (argc != 2) {
    printf("Usage %s <amplitude>\n", argv[0]);
    return 0;
    }

  amp = atoi(argv[1]);

  ret = setamp(amp);

  if (ret) {
    printf("Error setting amplitude %d\n", amp);
    return(0);
    }

  return(0);
}

