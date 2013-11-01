#!/bin/bash
HTML="rdcurves.html"
echo -ne "Writing all PNG graphs to HTML file \"$HTML\" ... "
echo "<!DOCTYPE html>" >> $HTML
echo "<html>" >> $HTML
echo "<body>" >> $HTML
for f in *.png
do
  echo "<img src=$f width=\"900\" height=\"600\" />" >> $HTML
done
echo "</body>" >> $HTML
echo "</html>" >> $HTML
echo "done"

