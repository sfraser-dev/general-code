#!/bin/bash

# Using: 
# latex file.tex; dvipdf file.dvi 
# wouild compress all the eps files to pdf files on every
# execution, this would take a long time.
# Better to just convert once (using this script)
# and then use:
# pdflatex file.tex

for f in *.png
do 
  # convert each png file to an eps
  convert $f ${f/.png}.eps
done
for f in *.eps
do 
  # convert each eps to a pdf
  epstopdf $f 
done

