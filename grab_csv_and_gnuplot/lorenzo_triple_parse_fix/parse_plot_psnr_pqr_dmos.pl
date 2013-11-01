#!/usr/bin/perl -w

#----------------------------------------------------------------------------
##   Description:   Script for reading PQA CSV data (PSNR, PQR and DMOS results
##                  all in the same file, with the PSNR results by themselves in
#                   a table and the PQR/DMOS results togther in a seperate table)
##   Date:          Nov 2012
##   Author:        Stewart    
##----------------------------------------------------------------------------

use strict;
use diagnostics;

my $num_args;                                                                        
my $prog_name;
my $LOGFILE;
my $LF;
my $PSNRFILE;
my $PQRFILE;
my $DMOSFILE;
my $line;

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
open $PSNRFILE, '>', 'psnr.dat' or die "Couldn't open file 'pqr.dat': $!";
open $PQRFILE, '>', 'pqr.dat' or die "Couldn't open file 'pqr.dat': $!";
open $DMOSFILE, '>', 'dmos.dat' or die "Couldn't open file 'dmos.dat': $!";

# Grab PQR and write to file.
open $LF, '<', $LOGFILE or die "Couldn't open file '$LOGFILE': $!";
while ( $line = <$LF> ) {                                                          
    # Only PSNR data lines contain ",--,--".
    if ( $line =~ /,--,--,/ && $line !~ /Overall/ ) {
        my @fields=split (",",$line);
        # PSNR is the 10th column (last column), of the first table.
        # The $fields[9] element contains a carridge return after the PSNR
        # value.
        print $PSNRFILE "$fields[9]";                                                             
    }                                                                                  
}   
close($LF);

# Grab PQR and write to file.
open $LF, '<', $LOGFILE or die "Couldn't open file '$LOGFILE': $!";
while ( $line = <$LF> ) {                                                          
    # Look for lines containing ",--", as these have the PQR and DMOS data.
    # Only PSNR data lines contain ",--,--".
    if ( $line =~ /,--/ && $line !~ /,--,--/ && $line !~ /Overall/ ) {
        my @fields=split (",",$line);
        # PQR is the 9th column of the second table.
        print $PQRFILE "$fields[8]\n";                                                             
    }                                                                                  
}   
close($LF);

# Grab DMOS and write to file.
open $LF, '<', $LOGFILE or die "Couldn't open file '$LOGFILE': $!";
while ( $line = <$LF> ) {                                                          
    # Look for lines containing ",--", as these have the PQR and DMOS data.
    # Only PSNR data lines contain ",--,--".
    if ( $line =~ /,--/ && $line !~ /,--,--/ && $line !~ /Overall/ ) {
        my @fields=split (",",$line);
        # DMOS is the 8th column of the second table.
        print $DMOSFILE "$fields[7]\n";                                                             
    }                                                                                  
}   
close($LF);

exit 0;

