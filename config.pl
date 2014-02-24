#!/usr/bin/perl

$tablebits = 13;
# How many bits should be used to address the wavetable?
# The number of samples in the wavetable will be two raised
# to this power, maximum 13.  13=>8192, 12=>4096, etc

$ddsbits = 8;
# how many bits should the DDS output?
# Choose 1 to 8 (perhaps more later)

$cpucycles = 7;
# How many CPU cycles in the DDS main loop? (see dds.p)

$clockrate = 200000000;
# CPU cycles per second of the PRU.
# This shouldn't change unless youre not on a beaglebone black

$prunum = 0;
# Which PRU are we to run the DDS code on?

