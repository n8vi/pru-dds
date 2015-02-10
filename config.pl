#!/usr/bin/perl

# Copyright (c) 2014, 2015 Brad J. Tarratt

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

$graballpins = 1;
# Change to zero if you need some of the pins unused by the DDS for something
# else.  This will probably break future code I write that uses GPIO output 
# from both PRUs (I'm thinking of an I+Q modulator, for one).

$tablebits = 12;
# How many bits should be used to address the wavetable?
# The number of samples in the wavetable will be two raised
# to this power, maximum 13.  13=>8192, 12=>4096, etc
# rerun startdds and one of the wavetable loaders after changing this

$ddsbits = 8;
# how many bits should the DDS output?
# Choose 1 to 8 (perhaps more later)
# reboot and run make test after changing this

$cpucycles = 7;
# How many CPU cycles in the DDS main loop? (see dds.p)
# rerun startdds after changing this

$clockrate = 200000000;
# CPU cycles per second of the PRU.
# This shouldn't change unless youre not on a beaglebone black

$mainprunum = 1;
# Which PRU are we to run the DDS code on by default?
# rerun startdds after changing this

$realtimefreq = 1;
# glitch-free frequency changes (in exchange for lower max frequency)
# rerun startdds and setfreq after changing this
# you probably want this set to 1 if you intend to use the "sweep" program

$amplitude = 1;
# At the expense of max frequency, this allows you to set amplitude

$realtimeamp = 1;
# glitch-free amplitude changes (in exchange for lower max frequency)
# rerun startdds and setfreq after changing this

