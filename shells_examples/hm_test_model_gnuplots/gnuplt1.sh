#!/bin/bash

BASENAME_1=$1
BASENAME_2=$2
OUTPUTNAME="rdcurve.png"

# gnuplot rd-curve
echo "set terminal png size 1200, 800; set output '$OUTPUTNAME'; \
  set key top left; \
  set xrange [0:410]; set yrange [25:45]; \
  set xlabel 'Bitrate (kbps)'; set ylabel 'PSNR (dB)'; \
  set title 'Rate Distortion Curve'; set grid; \
  plot '$BASENAME_1' with lines, \
       '$BASENAME_2' with lines;" | gnuplot

