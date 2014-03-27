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

	MOV	r5.w0, 8191
	MOV	r5.w1, 8191
	MOV	r6.w0, 2
	MOV	r6.w1, 2
	MOV	r1.w0, 1
	MOV	r1.w1, 0
	MOV	r7, PRU0_ARM_INTERRUPT+16
	LSL	r7, r7, 12

SPIN:

	LBCO	r30.b0, CONST_PRUDRAM, r1.w0, 1		// 3  1
	ADD	r1, r1, r6				// 1  4
	AND	r1, r1, r5				// 1  5

	XOR	r3, r1.w1, r1.w0			// 1  6
	ADD	r4, r1.w1, r7				// 1  7
	LBCO	r30.b0, CONST_PRUDRAM, r1.w1, 1		// 3  8
	LSR	r4, r4, 12				// 1  11
	QBBS	INT, r3, 12			// 1  1 =  3    
	QBA	NOINT				// 2  x ==    14
INT:
 	MOV	r31.b0, r4			// x  2
NOINT:
	QBA	SPIN				// 3  3
