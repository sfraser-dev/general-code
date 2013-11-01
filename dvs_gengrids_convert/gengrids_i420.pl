#!/usr/bin/perl -w
#
# Process the arguments.
#
if ($#ARGV != 2)
{
    die("Usage: gengrids <output> <pels> <lines>\n");
}

my $output = $ARGV[0];
my $pels   = $ARGV[1];
my $lines  = $ARGV[2];
my $buff;
my @temp;

#
# Open the files.
#
open(OFILE, ">$output") or die("Couldnt open output file!\n");
binmode(OFILE);

#
#
#
&gengrid(10, 1, 5, 255, 128, 128);
&gengrid(1, 1, 0, 255, 128, 128);
&gengrid(11, 1, 5, 255, 128, 128);
&gengrid(1, 1, 0, 255, 128, 128);
&gengrid(12, 1, 5, 255, 128, 128);
&gengrid(1, 1, 0, 255, 128, 128);
&gengrid(10, 1, 5, 255, 128, 128);
&gengrid(2, 1, 0, 255, 128, 128);
&gengrid(11, 1, 5, 255, 128, 128);
&gengrid(2, 1, 0, 255, 128, 128);
&gengrid(12, 1, 5, 255, 128, 128);
&gengrid(2, 1, 0, 255, 128, 128);

&gengrid(20, 32, 0, 255, 128, 128);
&gengrid(20, 16, 0, 255, 16, 128);
&gengrid(20, 4, 0, 255, 128, 16);
&gengrid(20, 2, 0, 255, 16, 16);
&gengrid(20, 1, 0, 255, 255, 255);

&gengrid(20, 32, 0, 255, 128, 128);
&gengrid(20, 16, 0, 255, 16, 128);
&gengrid(20, 4, 0, 255, 128, 16);
&gengrid(20, 2, 0, 255, 16, 16);
&gengrid(20, 1, 0, 255, 255, 255);

&gengrid(130, 1, 0, 255, 128, 128);
&gengrid(130, 1, 0, 255, 255, 255);

&gengrid(20, 1, 0, 255, 128, 128);
&gengrid(20, 1, 1, 255, 128, 128);
&gengrid(20, 1, 2, 255, 16, 128);
&gengrid(20, 1, 3, 255, 16, 128);

#
# Loop over the frames.
#
sub gengrid()
{
    my $frames  = $_[0];
    my $grid    = $_[1];
    my $type    = $_[2];
    my $y       = $_[3];
    my $cb      = $_[4];
    my $cr      = $_[5];
    my $cgrid   = $grid / 2;
    
    for ($k = 0; $k < $frames; $k++)
    {
        print("Frame $k\n");
    
        #
        # The luma.
        #
        for ($j = 0; $j < $lines; $j++)
        {
            for ($i = 0; $i < $pels; $i++)
            {
                $temp[$i] = 0;
                if (($type == 0 || $type == 1) && (($i / $grid) % 2))
                {
                    $temp[$i] = $y;                    
                }
                if (($type == 0 || $type == 2) && (($j / $grid) % 2))
                {
                    $temp[$i] = $y;                    
                }
            }
            $buff = pack("C*", @temp);
            syswrite(OFILE, $buff, $pels, 0);
        }
    
       #
       # The Cb.
       #
       for ($j = 0; $j < $lines / 2; $j++)
       {
            for ($i = 0; $i < $pels / 2; $i++)
            {
                $temp[$i] = 128;
                if (($type == 0 || $type == 1) && (($i / $cgrid) % 2))
                {
                    $temp[$i] = $cb;
                }
                if (($type == 0 || $type == 2) && (($j / $cgrid) % 2))
                {
                    $temp[$i] = $cb;
                }
            }
            $buff = pack("C*", @temp);
            syswrite(OFILE, $buff, $pels / 2, 0);
       }
    
       #
       # The Cr.
       #
       for ($j = 0; $j < $lines / 2; $j++)
       {
            for ($i = 0; $i < $pels / 2; $i++)
            {
                $temp[$i] = 128;
                if (($type == 0 || $type == 1) && (($i / $cgrid) % 2))
                {
                    $temp[$i] = $cr;
                }
                if (($type == 0 || $type == 2) && (($j / $cgrid) % 2))
                {
                    $temp[$i] = $cr;
                }
            }
            $buff = pack("C*", @temp);
            syswrite(OFILE, $buff, $pels / 2, 0);
       }
    }
}
