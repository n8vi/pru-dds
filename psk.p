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

/*******************************************************************************
So far, this file is just a copy of wobbulator.p.  The plan is to have a PRU
program to do binary PSK that has a way for the user to set symbol rate, basically
the time to transition through one cycle of the raised cosine filter, or from 
0x00002250 to 0x00002750 in the wavetable (or vice versa); and a way to signal a 
phase reversal should occur NOW.  One a phase transition begins, it should be
unstoppable and further requests for a phase transition should be ignored until
the current one is complete.  Should be pretty simple, the hard part is really
deciding when we're done with a phase reversal.  Perhaps when 
curpos - where_i_should_be < skiplen?  The second branch of that (the else)
doesn't need to contain the exact same cycle count as the first, and can just
set whatever flag initiates a reversal to zero.

This file has nothing to do with pskstream.c.  That file uses the DSB modulator,
but I may get it to talk to this instead if I get this working well enough.
********************************************************************************/

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
	MOV	R1, 0
	MOV	R2, 0
	MOV	R3, 0
	MOV	R4, 0

SPIN:
        LBBO    r3, r12, 0, 4 // Load skipcount from DRAM	// 3
	ADD	R2, R2, R3					// 1		
	MOV	R1, R2						// 1
	LSR	R1, R1, TABLEPREC				// 1

	MOV	R0, 0						// 1
	LBBO	R0.b0, R7, R1, 1				// 3
	SUB	R0, R0, 128					// 1

	SBBO	R0, R6, 4, 4	 				// 3

	MOV	R0, R0
	MOV	R0, R0
	MOV	R0, R0
	MOV	R0, R0
	MOV	R0, R0

	QBA	SPIN						// 1
							//	== 15
