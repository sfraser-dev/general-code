#!/usr/bin/perl -w                                                                                                          
#----------------------------------------------------------------------------
#   Description:    Script for reading and plotting the DMOS data from a PQA
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
    print ("Error! Usage is: perl $prog_name pqa.log\n");                       
    exit 1;                                                                            
}   

# File handling.                                                                     
$LOGFILE= $ARGV[0];                                                            
if (! -e $LOGFILE) {                                                            
    print ("Input log file doesn't exist, exiting.\n");                                
    exit 1;                                                                            
}        
open $LF, '<', $LOGFILE or die "Couldn't open file '$LOGFILE': $!";
open $DATAFILE, '>', 'dmos.dat' or die "Couldn't open file 'dmos.dat': $!";

while ( $line = <$LF> ) {                                                          
    # Look for lines starting with "#" and containing ",--,", as these have the 
    # PSNR, PQR and DMOS # results.
    if ( $line =~ /,--,/ && $line =~ /^#/ ) {
        my @fields=split (",",$line);
        # DMOS is the 7th column.
        print $DATAFILE "$fields[6]\n";                                                             
    }                                                                                  
}   

close($LF);
close($DATAFILE);

# Gnuplot PNG (be careful about the EOPLOT spacing)
my $GNUFILE = "dmos.dat";                                                                            
open (GNUPLOT, "|gnuplot");                                                          
print GNUPLOT <<EOPLOT;                                                              
set terminal png size 1200,800                                                       
set output "dmos.png"                                                              
set xrange [0:]                                                                      
set yrange [-10:]                                                                    
set xlabel "PQA Sample"                                                   
set ylabel "DMOS"                                                               
set title "Plot of DMOS"                                                           
set grid                                                                             
plot "$GNUFILE" w lines                                                              
EOPLOT

close(GNUPLOT);

exit 0;
