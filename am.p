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

#define CONST_PRUCFG	C4
#define CONST_PRUDRAM	C24
#define CTBIR_0		0x22020
#define CTPPR_1		0x2202C

#define PRU0_PRU1_INTERRUPT     17
#define PRU1_PRU0_INTERRUPT     18
#define PRU0_ARM_INTERRUPT      19
#define PRU1_ARM_INTERRUPT      20
#define ARM_PRU0_INTERRUPT      21
#define ARM_PRU1_INTERRUPT      22

#define GPIO1 0x4804c000
#define GPIO_DATAOUT 0x13C

START:  
	// Enable OCP master port
	LBCO	r0, CONST_PRUCFG, 4, 4
	CLR	r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
	SBCO	r0, CONST_PRUCFG, 4, 4

	MOV	r0, 0
	MOV	r1, CTBIR_0
	SBBO	r0,r1,0,4     // set up C24 = &PRU0_DRAM

	MOV	R6, 0x00003000 // Other PRU DRAM
	MOV 	r1, 0
	MOV	r5, 8191

	MOV	R7, 4096
	MOV	R8, 127

SPIN:
	MOV	r4, 12489					// 1
DELAY:
	SUB	r4, r4, 1					// 12491
	QBNE	DELAY, r4, 0					// 12491
	MOV	r0, r0						// 1
	MOV	r0, 0						// 1
	LBCO	r0, CONST_PRUDRAM, r1, 1			// 3
	SBCO	r8, CONST_PRUDRAM, r1, 1			// 3
	LSR	r0, r0, 1					// 1
	SBBO	r0, R6, 4, 4					// 3
	MOV	r2, r1						// 1
	AND	r2, r2, r7					// 1
	ADD	r1, r1, 1					// 1
	AND	r1, r1, r5
	MOV	r3, r1						// 1
	AND	r3, r3, r7					// 1
	QBNE	INT, r2, r3			// 1  1		// 3
	QBA	NOINT				// 2  x	//	== 16
INT:
 	MOV	r31.b0, PRU0_ARM_INTERRUPT+16	// x  2
NOINT:
	QBA	SPIN				// 3  3
