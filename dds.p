
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

#include "config.h"

.origin 0
.entrypoint START

#define CONST_PRUCFG	C4
#define CONST_PRUDRAM	C24
#define CONST_DDR	C31
#define CTBIR_0		0x22020
#define CTPPR_1		0x2202C

START:
	// Enable OCP master port (PRU will hard lockup when you try to access DDR if you don't)
	LBCO      r0, CONST_PRUCFG, 4, 4
	CLR     r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
	SBCO      r0, CONST_PRUCFG, 4, 4

	MOV	r10, 0
	MOV	r11, CTBIR_0
	SBBO    r10,r11,0,4     // set up C24 = &PRU0_DRAM

	MOV     r10, 0x00100000
	MOV	r11, CTPPR_1
	SBBO    r10,r11,0,4     // set up C31 = &DDR

#ifdef AMPLITUDE
        MOV     R25, 0          // Put the MAC in multiply-only mode
        XOUT    0, R25, 1
        MOV     R24, 16384
#endif

#ifndef REALTIMEAMP
        LBCO    r28, CONST_DDR, 4, 4 // Load amplitude from DDR
#endif

	MOV	r12, 4096

#ifndef REALTIMEFREQ
	LBCO	r3, CONST_DDR, 0, 4 // Load skipcount from DDR
#endif

// timing relies on the length of this loop.  Change config.h if you change this.
SPIN: 
#ifdef REALTIMEAMP
	LBBO	r28, r12, 4, 4 // Load amplitude from DRAM // 3
	// LBCO	r3, CONST_PRUDRAM, r12, 4 // Load skipcount from DRAM // 3
#endif
#ifdef REALTIMEFREQ
	LBBO	r3, r12, 0, 4 // Load skipcount from DRAM // 3
	// LBCO	r3, CONST_PRUDRAM, r12, 4 // Load skipcount from DRAM // 3
#endif
	ADD	r2, r2, r3			//  1
	MOV	r1, r2				// +1
	LSR	r1, r1, TABLEPREC		// +1

#ifdef AMPLITUDE
	MOV	R29, 0						// 1
        LBCO    R29.b0, CONST_PRUDRAM,r1, 1			// 3
	SUB	R29, R29, 128					// 1
        MOV     R0, R0 // have to wait 1 cycle for multiplier	// 1
        XIN     0, R26, 8					// 1
        ADD     R26, R26, R24					// 1
        LSR     r26, r26, 7					// 1
	MOV	r30.b0, r26					// 1
#endif							//	== 10 addtl

#ifndef AMPLITUDE
	LBCO	r30.b0, CONST_PRUDRAM, r1, 1	// +3
#endif

	QBA	SPIN				// +1
						// ===
						//  7 cpu cycles! ah! ah! ah!
