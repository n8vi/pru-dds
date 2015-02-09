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
	MOV	R12, 4096

	MOV	R0, 0 
	MOV	R1, 0 // CURPOS
	MOV	R2, 0 // CURPOS << TABLEPREC
	MOV	R3, 0 // SKIPLEN
	MOV	R4, 0 
        MOV     R11, 1

SPIN:
        LBBO    r3, r12, 0, 4 // Load skipcount from DRAM	// 3
	ADD	R2, R2, R3					// 1		
	MOV	R1, R2						// 1
	LSR	R1, R1, TABLEPREC				// 1

	MOV	R0, 0						// 1
	LBBO	R0.b0, R7, R1, 1				// 3
	SUB	R0, R0, 128					// 1

	SBBO	R0, R6, 4, 4	 				// 3

        LSR     R10, R1, TABLEBITSMINUSTWO
        LSL     R30, R11, R10
	MOV	R0, R0
	MOV	R0, R0
	MOV	R0, R0

	QBA	SPIN						// 1
							//	== 15
