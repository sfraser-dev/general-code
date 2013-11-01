#!/usr/bin/perl -w
use strict;
use diagnostics; 


my $LFD1;
my $LFD1C;
my $LOGFILE_ORIG;
my $LOGFILE_DEL1;
my $LOGFILE_DEL1_CONF;
my $prog_name;
my $num_args;
my $line;

# Command line checking.
$num_args = $#ARGV + 1;
if ($num_args != 1) {
  $prog_name = $0;
  print ("Error! Usage is: $prog_name svp_serial_psnr.log\n");
  exit 1;
}

# File handling.
$LOGFILE_ORIG = $ARGV[0];
if (! -e $LOGFILE_ORIG) {
  print ("Input log file doesn't exist, exiting.\n");
  exit 1;
}
$LOGFILE_DEL1= "tmp_del1.txt";
$LOGFILE_DEL1_CONF= "tmp_del1_conf.txt";
# Remove first blank line from SVP serial log file.
system("sed '1d' $LOGFILE_ORIG > $LOGFILE_DEL1"); 
# Open the files.
open $LFD1, '<', $LOGFILE_DEL1 or die "Couldn't open file '$LOGFILE_DEL1': $!";
open $LFD1C, '>', $LOGFILE_DEL1_CONF or die "Couldn't open file '$LOGFILE_DEL1_CONF': $!";

while ( $line = <$LFD1> ) {
  if ( $line =~ /ms # psnr: y</ ) {
    print $LFD1C "$line";
  }
}
close($LFD1);
close($LFD1C);
exit 0;

