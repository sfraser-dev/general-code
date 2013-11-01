#!/usr/bin/perl -w
use strict;
use diagnostics; 

# Take a SVP serial port log file of PSNR values (mixed with other information).
# Extracts the PSNR values delimited by "<" and ">" and prints them out to a CSV
# file (format: psnr_y, psnr_ya, psnr_c, psnr_ca). The first column of this CSV
# file (psnr_y) is cut and is output as a gnu-plotted PNG image. All columns 
# (PSNRs) are cut from the CSV and are output as .dat single column files.

#my $LFD1;
my $LFOH;
my $LFCH;
my $PH;
my $PYH;
my $SH;
my $LOGFILE_ORIG;
my $LOGFILE_CONFORM;
my $PSNRFILE;
my $SORTEDFILE;
my @split1;
my @split_tmp;
my @sort_arr;
my $arr_size;
my $prog_name;
my $num_args;
my $line;
my $psnr_y;
my $psnr_ya;
my $psnr_c;
my $psnr_ca;
my $psnr_y_min;
my $psnr_y_max;
my $psnr_y_avg;

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
$LOGFILE_CONFORM= "tmp_conform.txt";
$PSNRFILE = "psnr.csv";
$SORTEDFILE = "tmp_sorted.txt";
# Remove first blank line from SVP serial log file (usually an incomplete "broken" line).
#system("sed '1d' $LOGFILE_ORIG > $LOGFILE_DEL1"); 
# Open the files.
#open $LFD1, '<', $LOGFILE_DEL1 or die "Couldn't open file '$LOGFILE_DEL1': $!";
open $LFOH, '<', $LOGFILE_ORIG or die "Couldn't open file '$LOGFILE_ORIG': $!";
open $LFCH, '>', $LOGFILE_CONFORM or die "Couldn't open file '$LOGFILE_CONFORM': $!";
open $PH, '>', $PSNRFILE or die "Couldn't open file '$PSNRFILE': $!";

# Making sure the log file conforms to what is expected.
# Discard the first two lines from SVP serial log file (usually a blank or an incomplete "broken" line).
<$LFOH>; # Line 1
<$LFOH>; # Line 2
while ( $line = <$LFOH> ) {
  # Make sure the log file conforms to what is expected and doesn't have "broken" lines.
  # An expected PSNR log line from the SVP should look like:
  # 043 ms # psnr: y<34.33>, ya<37.42>, c<36.35>, ca<38.73> |=======
  if ( $line =~ /ms # psnr: y</ && $line =~ />, ya</ && $line =~ />, c</ && $line =~ />, ca</ ) {
    print $LFCH "$line";
  }
}
close ($LFOH);
# Close the conforming log file for writing as will now read from it.
close ($LFCH);
# Now open the conforming log file to grab the PSNR values.
open $LFCH, '<', $LOGFILE_CONFORM or die "Couldn't open file '$LOGFILE_CONFORM': $!";
# Open "psnr_y.dat" for writing the psnr_y values.
open $PYH, '>', 'psnr_y.dat' or die "Couldn't open file 'psnr_y.dat': $!";

# Splitting out the PSNR values from the SVP serial log to a CSV file.
while ( $line = <$LFCH> ) {
  # @split1 contains 5 elements. Element 0=unwanted, 
  # elements 1,2,3 and 4=psnr values for y, ya, c, and ca respectively.
  @split1= split('<',$line);

  # Initially splitting $split[1] which contains the first PSNR value. 
  # $split_tmp[0] will then contain the PSNR value.
  @split_tmp = split('>', $split1[1]); 
  $psnr_y = $split_tmp[0];
  @split_tmp = split('>', $split1[2]);
  $psnr_ya = $split_tmp[0];
  @split_tmp = split('>', $split1[3]);
  $psnr_c = $split_tmp[0];
  @split_tmp = split('>', $split1[4]);
  $psnr_ca = $split_tmp[0];

  # Print PSNR values to a comma separated file.
  print $PH "$psnr_y,$psnr_ya,$psnr_c,$psnr_ca\n";
  print $PYH "$psnr_y\n";
}

# Cleanup.
close($PH);
close($PYH);
close($LFCH);

#system("cut -f1 -d, $PSNRFILE > psnr_y.dat");
#system("cut -f2 -d, $PSNRFILE > psnr_ya.dat");
#system("cut -f3 -d, $PSNRFILE > psnr_c.dat");
#system("cut -f4 -d, $PSNRFILE > psnr_ca.dat");

# Find min, max and average of psnr_y.dat (now reading from "psnr_y.dat").
open $PYH, '<', 'psnr_y.dat' or die "Couldn't open file 'psnr_y.dat': $!";
open $SH, '>', $SORTEDFILE or die "Couldn't open file 'sorted.tmp': $!";
@sort_arr = <$PYH>;
@sort_arr = sort @sort_arr;
$arr_size = @sort_arr;
foreach $line (@sort_arr) {
  print $SH "$line";
}
$psnr_y_min = $sort_arr[0];
$psnr_y_max = $sort_arr[$arr_size-1];
$psnr_y_avg = 0;
($psnr_y_avg+=$_) for @sort_arr;
$psnr_y_avg /= $arr_size;
print " arr_size=$arr_size\n psnr_y_min=$psnr_y_min psnr_y_max=$psnr_y_max psnr_y_avg=$psnr_y_avg\n";
close($SH);
close($PYH);

# Gnuplot PNG of psnr_y.dat
my $GNUFILE = "psnr_y.dat";
open (GNUPLOT, "|gnuplot");
print GNUPLOT <<EOPLOT;
set terminal png size 1200,800
set output "psnr_y.png"
set xrange [0:]
set yrange [-10:]
set xlabel "SVP Serial Log Sample"
set ylabel "PSNR (dB)"
set title "PSNR Plot of Y"
set grid
plot "$GNUFILE" w lines
EOPLOT
close(GNUPLOT);

exit 0;
