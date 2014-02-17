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
	// ST32	r10, r11
	SBBO    r10,r11,0,4     // set up C24 = &PRU0_DRAM

	MOV     r10, 0x00100000
	MOV	r11, CTPPR_1
	// ST32	r10, r11
	SBBO    r10,r11,0,4     // set up C31 = &DDR
	LBCO	r3, CONST_DDR, 0, 4 // Load skipcount from DDR

SPIN:
	ADD	r2, r2, r3			//  1
	MOV	r1, r2				// +1
	LSR	r1, r1, 19			// +1
	LBCO	r30.b0, CONST_PRUDRAM, r1, 1	// +3
	QBA	SPIN				// +1
						// ===
						//  7 cpu cycles! ah! ah! ah!
