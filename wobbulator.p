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
	MOV	R7, 0x00002000 // The WAVETABLE
	MOV	R8, 0xFFFFFFFF
	MOV	R9, 4095


SPIN:
	ADD	R2, R2, 1
	AND	R2, R2, R9
	MOV	R0, 0
	LBBO	R0.b0, R7, R2, 1
	SUB	R0, R0, 128
	// LSR	R0, R0, 1
	// AND	R0, R0, 127    // R14 = R14 & 0x0fff
	SBBO	R0, R6, 4, 4	 // [00003000 + 4] = R12

	MOV	R5, 1000
DEL:
	SUB	R5, R5, 1
	QBNE	DEL, R5, 0

	QBA	SPIN

        // Send notification to Host for program completion
        // MOV     r31.b0, PRU0_ARM_INTERRUPT+16

        // Halt the processor
        // HALT

