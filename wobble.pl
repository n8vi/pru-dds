#!/usr/bin/perl

$pi = 3.141592653585;

while(1) {
  print "wobble\n";
  for ($i=0; $i<2*$pi; $i+=0.01) {
    $freqofs = sin($i)*500;
    $freq = 1850000+$freqofs;
    `./setfreq $freq`;
    }
  }
