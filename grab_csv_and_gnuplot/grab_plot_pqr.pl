#!/usr/bin/perl -w                                                                                                          
#----------------------------------------------------------------------------
#   Description:    Script for reading and plotting the PQR data from a PQA
#   generated CSV file
#   Date:           Nov 2012
#   Author:         Stewart    
#----------------------------------------------------------------------------
use strict;                                                                          
use diagnostics;                                                                     
                                                                                       
my $num_args;                                                                        
my $prog_name;                                                                       
my $LOGFILE;
my $LF;
my $line;
my $DATAFILE;

# Command line checking.                                                             
$num_args = $#ARGV + 1;                                                              
if ($num_args != 1) {                                                                
    $prog_name = $0;                                                                   
    print ("Error! Usage is: perl $prog_name pqr_from_pqa.csv\n");                       
    exit 1;                                                                            
}   

# File handling.                                                                     
$LOGFILE= $ARGV[0];                                                            
if (! -e $LOGFILE) {                                                            
    print ("Input log file doesn't exist, exiting.\n");                                
    exit 1;                                                                            
}        
open $LF, '<', $LOGFILE or die "Couldn't open file '$LOGFILE': $!";
open $DATAFILE, '>', 'pqr.dat' or die "Couldn't open file 'pqr.dat': $!";

while ( $line = <$LF> ) {                                                          
    # Look for lines starting with "#" and containing ",--,", as these have the 
    # PSNR, PQR and DMOS # results.
    if ( $line =~ /,--,/ && $line =~ /^#/ ) {
        my @fields=split (",",$line);
        # PQR is the 8th column.
        print $DATAFILE "$fields[7]\n";                                                             
    }                                                                                  
}   

close($LF);
close($DATAFILE);

# Gnuplot PNG (be careful about the EOPLOT spacing)
my $GNUFILE = "pqr.dat";                                                    
open (GNUPLOT, "|gnuplot");                                                          
print GNUPLOT <<EOPLOT;                                                              
set terminal png size 1200,800                                                       
set output "pqr.png"                                                              
set xrange [0:]                                                                      
set yrange [-10:]                                                                    
set xlabel "PQA Sample"                                                   
set ylabel "PQR"                                                               
set title "Plot of PQR"                                                           
set grid                                                                             
plot "$GNUFILE" w lines                                                              
EOPLOT

close(GNUPLOT);

exit 0;
