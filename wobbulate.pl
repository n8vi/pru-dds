#!/usr/bin/perl

$pi = 3.141592653585;

while(1) {
  print "wobbulate\n";
  for ($i=0; $i<2*$pi; $i+=0.01) {
    $amp = sin($i)*127;
    `./setamp $amp`;
    }
  }
