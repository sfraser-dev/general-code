#!/bin/bash

for f in *.png
do 
  # convert each png file to an eps
  convert $f ${f/.png}.eps
done
