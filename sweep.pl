#!/usr/bin/perl

# this isn't *totally* clean, as "setskip" resets the PRU's instruction pointer
# in order to get the code to reload the skip interval

for ($i=30; $i<3000; $i++) {
  print `./setfreq $i`;
#  select(undef, undef, undef, 0.1);
  }
