#!/bin/bash

CLEANNAME=$1

DATFILEPSNR=${CLEANNAME/.csv}-psnr.dat
DATFILEPQR=${CLEANNAME/.csv}-pqr.dat
DATFILEDMOS=${CLEANNAME/.csv}-dmos.dat
sed -n '/Ref Frame/,$p' $CLEANNAME | sed '/Ref Frame/d' | cut -f8 -d, > $DATFILEDMOS
sed -n '/Ref Frame/,$p' $CLEANNAME | sed '/Ref Frame/d' | cut -f9 -d, > $DATFILEPQR
sed -n '/Ref Frame/,$p' $CLEANNAME | sed '/Ref Frame/d' | cut -f10 -d, > $DATFILEPSNR

dos2unix -q $DATFILEPSNR
sed '/^$/d' $DATFILEPSNR > datfile_unix_format_with_no_empty_spaces_with_summary_psnr.dat
head -n -2 datfile_unix_format_with_no_empty_spaces_with_summary_psnr.dat > $DATFILEPSNR  

dos2unix -q $DATFILEPQR
sed '/^$/d' $DATFILEPQR > datfile_unix_format_with_no_empty_spaces_with_summary_pqr.dat
head -n -2 datfile_unix_format_with_no_empty_spaces_with_summary_pqr.dat > $DATFILEPQR  

dos2unix -q $DATFILEDMOS
sed '/^$/d' $DATFILEDMOS > datfile_unix_format_with_no_empty_spaces_with_summary_dmos.dat
head -n -2 datfile_unix_format_with_no_empty_spaces_with_summary_dmos.dat > $DATFILEDMOS  

#AVGVAL=$(sed -n '/'$METRIC'/,$p' datfile_unix_format_with_no_empty_spaces_with_summary.dat | tail -1)
#echo "Average $METRIC = $AVGVAL"

#rm datfile_unix_format_with_no_empty_spaces_with_summary_psnr.dat 
#rm datfile_unix_format_with_no_empty_spaces_with_summary_pqr.dat 
#rm datfile_unix_format_with_no_empty_spaces_with_summary_dmos.dat 

