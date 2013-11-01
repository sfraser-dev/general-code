#! /bin/bash

# Simple shell to print the disk usage every regularly to track webHTTrack progress (which can take a while if tracked website is big and connection is slow)
while :
do 
  SIZE=`du -sh`
  DATE=$(date +"%y%m%d %H:%M.%S")
  echo "$SIZE $DATE"
  sleep 180
done
