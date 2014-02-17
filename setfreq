#!/usr/bin/perl

$tablelen = 8192;
$cpucycles = 7;
$clockrate = 200000000;

if (@ARGV+0 != 1) {
  die "usage: $0 <frequency>\n";
  }

$freq = $ARGV[0];

$skiplen = ($freq * $tablelen * $cpucycles) / $clockrate;

print `./setskip $skiplen`;
