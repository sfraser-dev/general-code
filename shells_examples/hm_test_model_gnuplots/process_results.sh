#!/bin/bash

if [ $# -lt 1 ]; then
  echo ""
  echo "Error: enter directory name on command line (date will be prepended)"
  echo ""
  exit 0
fi

DIR=$1

NOW=$(date +"%y%m%d")
DIR=$NOW"_"$DIR
if [ ! -d $DIR ]; then 
  mkdir "$DIR"
else
  echo ""
  echo "Error: directory already exists"
  echo ""
  exit 0
fi

echo -ne "Moving all YUV, txt, PNG and bin files to the new directory ... "
mv *.yuv *.txt *.png *.bin $DIR
echo "done"
echo -ne "Copying all dat files to new directory ... "
cp *.dat $DIR
echo "done"
