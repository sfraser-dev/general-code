#!/bin/bash

function gnuplot_rd1 {
  local DATFILEIN1=$1
  local YMIN=$2
  local YMAX=$3
  local PNGFILEOUT=${DATFILEIN1/.dat}.png
  # gnuplot rd-curve
  echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set xrange [0:]; set yrange [$YMIN:$YMAX]; \
    set xlabel 'Frame Number'; set ylabel 'PSNR (dB)'; set title 'PQA PSNR Plot'; set grid; \
    plot '$DATFILEIN1' with lines;" | gnuplot
}


if [ $# -lt 1 ]; then 
  echo ""
  echo "Usage: $0 PQA_PSNR.csv"
  echo ""
  exit 0
fi

MESSYNAME=$1

CLEANNAME=$(echo "$MESSYNAME" | sed 's/ /_/g' | sed 's/(//g' | sed 's/)//g')
cp "$MESSYNAME" $CLEANNAME

DATFILE=${CLEANNAME/.csv}.dat
sed -n '/Frame<Ref/,$p' $CLEANNAME | sed '/Frame<Ref/d' | sed 1d | cut -f9 -d, > $DATFILE
  
dos2unix -q $DATFILE
sed '/^$/d' $DATFILE > datfile_unix_no_empty_spaces_with_summary.dat
head -n -2 datfile_unix_no_empty_spaces_with_summary.dat > $DATFILE  
  
AVGPSNR=$(sed -n '/PSNR/,$p' datfile_unix_no_empty_spaces_with_summary.dat | tail -1)
echo "AVGPSNR=$AVGPSNR"

gnuplot_rd1 $DATFILE 0 100

rm "$CLEANNAME" datfile_unix_no_empty_spaces_with_summary.dat
