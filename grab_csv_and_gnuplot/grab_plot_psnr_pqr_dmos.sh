#!/bin/bash

function gnuplot_rd1 {
  local DATFILEIN=$1
  local YMIN=$2
  local YMAX=$3
  local METRIC=$4
  local PNGFILEOUT=${DATFILEIN/.dat}.png
  
  if [ "$METRIC" = "PSNR" ]; then 
    echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set xrange [0:]; set yrange [$YMIN:$YMAX]; \
    set xlabel 'Frame Number'; set ylabel 'PSNR (dB)'; set title 'PSNR Plot From PQA'; set grid; \
    plot '$DATFILEIN' with lines;" | gnuplot
  elif [ "$METRIC" = "PQR" ]; then 
    echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set xrange [0:]; set yrange [$YMIN:$YMAX]; \
    set xlabel 'Frame Number'; set ylabel 'PQR'; set title 'PQR Plot From PQA'; set grid; \
    plot '$DATFILEIN' with lines;" | gnuplot
  elif [ "$METRIC" = "DMOS" ]; then 
    echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set xrange [0:]; set yrange [$YMIN:$YMAX]; \
    set xlabel 'Frame Number'; set ylabel 'DMOS'; set title 'DMOS Plot From PQA'; set grid; \
    plot '$DATFILEIN' with lines;" | gnuplot
  else 
    echo""
    echo"Error: unknown metric in gnuplot_rd1()"
    echo""
  fi
}

for MESSYNAME in *.csv
do
  if [[ "$MESSYNAME" =~ "PSNR" ]]; then 
    METRIC="PSNR"
  elif [[ "$MESSYNAME" =~ "PQR" ]]; then 
    METRIC="PQR"
  elif [[ "$MESSYNAME" =~ "DMOS" ]]; then 
    METRIC="DMOS"
  else 
    echo""
    echo"Error: unknown input file / metric type."
    echo""
  fi

  CLEANNAME=$(echo "$MESSYNAME" | sed 's/ /_/g' | sed 's/(//g' | sed 's/)//g')
  cp "$MESSYNAME" $CLEANNAME

  DATFILE=${CLEANNAME/.csv}.dat
  if [ $METRIC = "PSNR" ]; then 
    sed -n '/Frame<Ref/,$p' $CLEANNAME | sed '/Frame<Ref/d' | cut -f9 -d, > $DATFILE
  elif [ $METRIC = "PQR" ]; then 
    sed -n '/Frame<Ref/,$p' $CLEANNAME | sed '/Frame<Ref/d' | cut -f8 -d, > $DATFILE
  elif [ $METRIC = "DMOS" ]; then 
    sed -n '/Frame<Ref/,$p' $CLEANNAME | sed '/Frame<Ref/d' | cut -f7 -d, > $DATFILE
  else
    echo""
    echo"Error: unknown metric type."
    echo""
  fi
  
  dos2unix -q $DATFILE
  sed '/^$/d' $DATFILE > datfile_unix_format_with_no_empty_spaces_with_summary.dat
  head -n -2 datfile_unix_format_with_no_empty_spaces_with_summary.dat > $DATFILE  

  AVGVAL=$(sed -n '/'$METRIC'/,$p' datfile_unix_format_with_no_empty_spaces_with_summary.dat | tail -1)
  echo "Average $METRIC = $AVGVAL"

  gnuplot_rd1 $DATFILE 0 100 $METRIC

  rm "$CLEANNAME" datfile_unix_format_with_no_empty_spaces_with_summary.dat 
done
