#!/usr/bin/perl

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

$prunum = 0;
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
