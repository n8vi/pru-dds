
SHELL=/bin/bash # globs don't work if I don't (see install-dtbo target)
CC=gcc -Wall -D__DEBUG -O2 -mtune=cortex-a8 -march=armv7-a
LDLIBS=-lprussdrv -lpthread -lm
BINS=ramp sawtooth setfreq sine square startdds

all: dds.bin ${BINS} BB-BONE-PRUDDS-1-00A0.dtbo

${BINS} : % : %.o util.o
	${CC} -o $@ $< util.o ${LDLIBS}

%.bin: %.p
	pasm -b $<

%.dtbo: %.dts
	dtc -O dtb -o $@ -b 0 -@ $<

install-dtbo: BB-BONE-PRUDDS-1-00A0.dtbo
	cp BB-BONE-PRUDDS-1-00A0.dtbo /lib/firmware/
	echo BB-BONE-PRUDDS-1 > /sys/devices/bone_capemgr.*/slots || echo 'Error installing DTBO (already installed?)'

test: install-dtbo all
	sleep 1
	./sine
	./setfreq 10000
	./startdds

clean:
	rm -fr *.bib *.bin *~ *_bin.h *.o *.dtbo .*.swp ${BINS}
