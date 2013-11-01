#! /bin/bash

# Testing it on my Linux box first.
echo "673 0x11111" > file1
# Replace 0x with a newline character
sed s/0x/\\n/ <file1 >file2
# Grab the first line
oneval=$(head -n 1 file2)
# Calculate the temperature
tempval=$(echo "$oneval 503.975 * 1024 / 273.15 - p" | dc)
echo "$(date +"%y%m%d %H:%M.%S") FPGA temperature is: $tempval degrees celcius"
rm file1 file2
