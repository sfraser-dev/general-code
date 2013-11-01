#!/bin/bash

if ls *.txt &> /dev/null; then
  echo "*.txt do exist, deleting"
  rm *.txt
else 
  echo "*.txt files do not exist"
fi

if ls *.dat &> /dev/null; then
  echo "*.dat do exist, deleting"
  rm *.dat
else 
  echo "*.dat files do not exist"
fi

if ls *.csv &> /dev/null; then
  echo "*.csv do exist, deleting"
  rm *.csv
else 
  echo "*.csv files do not exist"
fi

if ls *.png &> /dev/null; then
  echo "*.png do exist, deleting"
  rm *.png
else 
  echo "*.txt files do not exist"
fi
#rm *.txt *.dat *.csv *.png
