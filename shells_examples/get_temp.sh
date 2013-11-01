#!/bin/sh

while :
do
  twovals=$(/opt/app/fpga_read32 0x2003)
  echo $twovals > file1
  sed s/0x/\\n/ <file1 >file2
  oneval=$(head -n 1 file2)
  tempval=$(echo "$oneval 503.975 * 1024 / 273.15 - p" | dc)
  echo "FPGA temperature is: $tempval degrees celcius"
  #date
  #/opt/app/fpga_read32 0x2030
  sleep 60
done

