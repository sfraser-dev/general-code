#!/usr/bin/perl -w
use strict;
use diagnostics; 

my $num_args;
my $prog_name;
my $CSVFILE;

# Command line checking.
$num_args = $#ARGV + 1;
if ($num_args != 1) {
  $prog_name = $0;
  print ("Error! Usage is: $prog_name stm_datastore_confidence.csv\n");
  exit 1;
}

# File handling.
$CSVFILE = $ARGV[0];
if (! -e $CSVFILE) {
  print ("Input log file doesn't exist, exiting.\n");
  exit 1;
}

#print "File is $CSVFILE\n";
system("cut -f3 -d, $CSVFILE | tac > confidence.dat");

# Gnuplot PNG of stm_confdence.dat
my $GNUFILE = "confidence.dat";
open (GNUPLOT, "|gnuplot");
print GNUPLOT <<EOPLOT;
set terminal png size 1200,800
set output "confidence.png"
set xrange [0:]
set yrange [0:10]
set xlabel "SVP datastore alignment confidences (sampled at 1 second intervals)"
set ylabel "Confidence"
set title "Plot of SVP Stereo Temporal Matching Confidence"
set grid
plot "$GNUFILE" w lines
EOPLOT
close(GNUPLOT);


exit 0;
