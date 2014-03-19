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

#include "config.h"

.origin 0
.entrypoint START

// #define CONST_PRUCFG    C4
// #define CONST_PRUDRAM   C24
#define CTBIR_0         0x22020
// #define CTPPR_1         0x2202C

#define CONST_PRUDRAM	C24

#define PRU0_ARM_INTERRUPT      19

START:  
	MOV	r0, 0
	MOV	r1, CTBIR_0
	SBBO	r0,r1,0,4     // set up C24 = &PRU0_DRAM

	MOV	r4, 4096
	MOV	r5, 8191
	MOV	r2, 0
	MOV	r1, 0

SPIN:
	LBCO	r30.b0, CONST_PRUDRAM, r1, 1		//  3  1
	ADD	r1, r1, 1				//  1  4
	AND	r1, r1, r5				//  1  5
	ADD	r2, r2, 1				//  1  6
	AND	r2, r2, r5				//  1  7
	LBCO	r30.b0, CONST_PRUDRAM, r2, 1		//  3  8
	XOR	r3, r2, r1				//  1  11
	QBGE	INT, r3, r4			// 1  1  =  3  12
	QBA	NOINT				// 2  x == 14
INT:
 	MOV	r31.b0, PRU0_ARM_INTERRUPT+16	// x  2
NOINT:
	QBA	SPIN				// 3  3
