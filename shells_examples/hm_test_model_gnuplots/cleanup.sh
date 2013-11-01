#!/bin/bash

echo "You really want to delete all YUV, bin, txt, dat, eps and PNG files? yes/no"
read INPUT 

if [ "$INPUT" == "yes" ]; then 
  rm *.yuv
  rm *.bin
  rm *.txt
  rm *.dat
  rm *.png
  rm *.eps
else
  echo "You said \"$INPUT\", so not deleting"
fi

