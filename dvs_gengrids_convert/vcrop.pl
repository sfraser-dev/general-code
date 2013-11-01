#!perl -w
# Crops a .yuv down in size
#
# Process the arguments.
#
if ($#ARGV != 8)
{
    die("Usage: vcrop <input> <output> <start> <end> <in pels> <in lines> <out pels> <out lines> 4:2:<2/0>\n");
}
my $input  = $ARGV[0];
my $output = $ARGV[1];
my $first  = $ARGV[2];
my $final  = $ARGV[3];
my $ipels  = $ARGV[4];
my $ilines = $ARGV[5];
my $opels  = $ARGV[6];
my $olines = $ARGV[7];
my $chroma = $ARGV[8];
my $buff;
my $offset = 2 * (($ipels - $opels) / 4);
my $skip   = 2 * (($ilines - $olines) / 4);
my $cskip;
my $cilines;
my $colines;

#
# Is this a 4:2:2 or a 4:2:0 sequence.
#
if ($chroma == 2)
{
    $cskip  = $skip;
    $colines = $olines;
    $cilines = $ilines;
}
else
{
    $cskip  = $skip / 2;
    $colines = $olines / 2;
    $cilines = $ilines / 2;
}

#
# Open the files.
#
open(IFILE, "<$input") or die("Couldnt open input file!\n");
binmode(IFILE);
open(OFILE, ">$output") or die("Couldnt open output file!\n");
binmode(OFILE);

#
# Loop over the frames.
#
for ($k = 0; $k < $final; $k++)
{
    print("Frame $k\n");

    #
    # The Y.
    #
    for ($j = 0; $j < $ilines; $j++)
    {
        sysread(IFILE, $buff, $ipels) or die("End of file at frame $k!");
        #if (($j >= $skip) && ($j < ($skip + $olines)) && ($k >= $first))
        if (($j < $olines) && ($k >= $first))
        {
            #syswrite(OFILE, $buff, $opels, $offset);
            syswrite(OFILE, $buff, $opels, 0);
        }

        # output width greater than input width.
        if ( $ipels < $opels)
        {
          for ($i = 0; $i < ($opels - $ipels); $i++)
          {
            syswrite(OFILE, 128, 1, 0);
          } 
        }
    }

    # output hight greater than input hight.
    if ( $k >= $first && $ilines < $olines)
    {
      for ($j = 0; $j < ($olines - $ilines); $j++)
      {
        for ($i = 0; $i < $opels; $i++)
        {
          syswrite(OFILE, 128, 1, 0);
        } 
      }
    }

    #
    # The Cb.
    #
    for ($j = 0; $j < $cilines; $j++)
    {
        sysread(IFILE, $buff, $ipels / 2) or die("End of file at frame $k!");
        #if (($j >= $cskip) && ($j < ($cskip + $colines)) && ($k >= $first))
        if (($j < $colines) && ($k >= $first))
        {
            #syswrite(OFILE, $buff, $opels / 2, $offset / 2);
            syswrite(OFILE, $buff, $opels / 2, 0);
        }

        # output width greater than input width.
        if ( $ipels < $opels)
        {
          for ($i = 0; $i < ($opels - $ipels) / 2; $i++)
          {
            syswrite(OFILE, 128, 1, 0);
          } 
        }
    }

    if ( $k >= $first && $cilines < $colines)
    {
      for ($j = 0; $j < ($colines - $cilines); $j++)
      {
        for ($i = 0; $i < $opels / 2; $i++)
        {
          syswrite(OFILE, 128, 1, 0);
        } 
      }
    }

    #
    # The Cr.
    #
    for ($j = 0; $j < $cilines; $j++)
    {
        sysread(IFILE, $buff, $ipels / 2) or die("End of file at frame $k!");
        #if (($j >= $cskip) && ($j < ($cskip + $colines)) && ($k >= $first))
        if (($j < $colines) && ($k >= $first))
        {
            #syswrite(OFILE, $buff, $opels / 2, $offset / 2);
            syswrite(OFILE, $buff, $opels / 2, 0);
        }

        # output width greater than input width.
        if ( $ipels < $opels)
        {
          for ($i = 0; $i < ($opels - $ipels) / 2; $i++)
          {
            syswrite(OFILE, 128, 1, 0);
          } 
        }
    }

    if ( $k >= $first && $cilines < $colines)
    {
      for ($j = 0; $j < ($colines - $cilines); $j++)
      {
        for ($i = 0; $i < $opels / 2; $i++)
        {
          syswrite(OFILE, 128, 1, 0);
        } 
      }
    }
}
