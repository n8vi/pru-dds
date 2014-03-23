# Copyright (c) 2014 Brad J. Tarratt

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

PREFIX?=/usr/local

SHELL=/bin/bash # globs don't work if I don't (see install-dtbo target)
CC=gcc -Wall -D__DEBUG -O2 -mtune=cortex-a8 -march=armv7-a
LDLIBS=-lprussdrv -lpthread -lm
PRUBINS=startwobbulator startdds startam startsam startdsb
CPUBINS=setfreq setamp getfreq getamp sweep wavetable haltpru modstream
TESTSRC=$(wildcard test*.c)
TESTBINS=$(patsubst %.c,%,$(TESTSRC))
TESTPRUBINS=$(patsubst %.c,%,$(wildcard starttest*.c))
PRUBINS+=$(TESTPRUBINS)
BINS=$(CPUBINS) $(PRUBINS) $(TESTBINS)
PRUOBJS=$(patsubst %,%.o,$(PRUBINS))

all: $(BINS) BB-BONE-PRUDDS-1-00A0.dtbo

$(BINS) : % : %.o util.o
	$(CC) -o $@ $< util.o $(LDLIBS)


%_bin.h: %.p config.h
	pasm -V2 -C$(basename $(notdir $<)) -c $<

$(PRUOBJS) : start%.o : start%.c config.h %_bin.h
	${CC} -c -o $@ $<

%.o : %.c config.h
	${CC} -c -o $@ $<

%.dtbo: %.dts
	dtc -O dtb -o $@ -b 0 -@ $<

#to load sound samples into the DDS wavetable
%.raw : %.wav
	sox $< -b 8 -u -c 1 -r 4096 -t raw $@

# to test the AM modulator
%.raw : %.mp3
	sox $< -b 8 -u -c 1 -r 8000 -t raw $@

%.sam : %.wav
	sox $< -b 8 -u -c 1 -r 28571429 -t raw $@

config.h: config.pl genfile.pl
	perl genfile.pl _config_h 

BB-BONE-PRUDDS-1-00A0.dts: config.pl genfile.pl
	perl genfile.pl _dts

install-dtbo: BB-BONE-PRUDDS-1-00A0.dtbo
	cp BB-BONE-PRUDDS-1-00A0.dtbo /lib/firmware/
	@echo echo BB-BONE-PRUDDS-1 \> /sys/devices/bone_capemgr.*/slots
	@echo BB-BONE-PRUDDS-1 > /sys/devices/bone_capemgr.*/slots || echo 'Error installing DTBO (already installed?)'

# To be enabled when binary names have stabilized (wobbulate, for example, may go away)
#
# install: install-dtbo all
#        install -m 0755 -d $(DESTDIR)$(PREFIX)/bin
#        install -m 0755 $(BINS) $(DESTDIR)$(PREFIX)/bin

test: install-dtbo all
	sleep 1
	./wavetable sine
	./setfreq 10000
	./setamp 127
	./startdds

clean:
	rm -fr *.bib *.bin *~ *_bin.h *.o *.dtbo .*.swp $(BINS) config.h BB-BONE-PRUDDS-1-00A0.dts
