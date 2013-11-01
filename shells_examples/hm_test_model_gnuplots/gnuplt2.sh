#!/bin/bash

function gnuplot_rd2 {
  local DATFILEIN1=$1
  local DATFILEIN2=$2
  local YMIN=$3
  local YMAX=$4
  local PNGFILEOUT=${DATFILEIN2/.dat}.png
  # gnuplot rd-curve
  echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set xrange [0:]; set yrange [$YMIN:$YMAX]; \
    set xlabel 'Bitrate (kbps)'; set ylabel 'PSNR (dB)'; set title 'Rate Distortion Curve'; set grid; \
    plot '$DATFILEIN1' with lines, \
         '$DATFILEIN2' with lines;" | gnuplot
  #set key top left; \
}

if [ $# -lt 2 ]; then 
  echo ""
  echo "Usage: $0 YMIN YMAX"
  echo ""
  exit 0;
fi

YMIN=$1
YMAX=$2

for f in *.dat 
# find the default / base configuration .dat files
do
  TEMP=$(echo $f | sed -n '/lowdelay_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ldm=$TEMP; fi
  TEMP=$(echo $f | sed -n '/lowdelay_P_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ldPm=$TEMP; fi
  TEMP=$(echo $f | sed -n '/randomaccess_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ram=$TEMP; fi
done

for f in *.dat 
# find the .dats with changed params and plot rd curves using their corresponding default / base .dat
do
  TEMP=$(echo $f | sed -n '/lowdelay_main/p' | sed -n '/frms\./!p'); 
  if [ ${#TEMP} -ne 0 ]; then 
    gnuplot_rd2 $BASE_ldm $TEMP $YMIN $YMAX; 
  fi
  TEMP=$(echo $f | sed -n '/lowdelay_P_main/p' | sed -n '/frms\./!p'); 
  if [ ${#TEMP} -ne 0 ]; then 
    gnuplot_rd2 $BASE_ldPm $TEMP $YMIN $YMAX; 
  fi
  TEMP=$(echo $f | sed -n '/randomaccess_main/p' | sed -n '/frms\./!p'); 
  if [ ${#TEMP} -ne 0 ]; then 
    gnuplot_rd2 $BASE_ram $TEMP $YMIN $YMAX; 
  fi
done
