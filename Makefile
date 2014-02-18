
SHELL=/bin/bash # globs don't work if I don't (see install-dtbo target)
CC=gcc -Wall -D__DEBUG -O2 -mtune=cortex-a8 -march=armv7-a
LDLIBS=-lprussdrv -lpthread -lm
BINS=ramp sawtooth setskip sine square startdds

all: dds.bin ${BINS}

${BINS} : % : %.o util.o
	${CC} -o $@ $< util.o ${LDLIBS}

dds.bin: dds.p
	pasm -b dds.p

install-dtbo: BB-BONE-PRU-8GPO-00A0.dtbo
	cp BB-BONE-PRU-8GPO-00A0.dtbo /lib/firmware/
	echo BB-BONE-PRU-8GPO > /sys/devices/bone_capemgr.*/slots || echo Error installing DTBO (already installed?)

BB-BONE-PRU-8GPO-00A0.dtbo: BB-BONE-PRU-8GPO-00A0.dts
	dtc -O dtb -o BB-BONE-PRU-8GPO-00A0.dtbo -b 0 -@ BB-BONE-PRU-8GPO-00A0.dts

test: install-dtbo all
	sleep 1
	./startdds

clean:
	rm -fr *.bib *.bin *~ *_bin.h *.o *.dtbo .*.swp ${BINS}
