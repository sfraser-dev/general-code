#!/bin/bash

function gnuplot_rd1 {
  local DATFILEIN=$1
  local PNGFILEOUT=${DATFILEIN/.dat}.png
  # gnuplot rd-curve
  echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set key top left; \
    set xrange [0:]; set yrange [25:50]; \
    set xlabel 'Bitrate (kbps)'; set ylabel 'PSNR (dB)'; set title 'Rate Distortion Curve'; set grid; \
    plot '$DATFILEIN' with lines;" | gnuplot
}

function gnuplot_rd2 {
  local DATFILEIN1=$1
  local DATFILEIN2=$2
  local PNGFILEOUT=${DATFILEIN2/.dat}.png
  # gnuplot rd-curve
  echo "set terminal png size 1200, 800; set output '$PNGFILEOUT'; \
    set key top left; \
    set xrange [0:]; set yrange [25:50]; \
    set xlabel 'Bitrate (kbps)'; set ylabel 'PSNR (dB)'; set title 'Rate Distortion Curve'; set grid; \
    plot '$DATFILEIN1' with lines, \
         '$DATFILEIN2' with lines;" | gnuplot
}

function gnuplot_rd2_automate {
  # Find the .dat files corresponding to the default / original configuration files. 
  # Find the .dat files with changed params and plot the changed param rdcurve alongside its corresponding original rdcurve. 
  for f in *.dat
  # find the default / original configuration .dat files
  do
    TEMP=$(echo $f | sed -n '/lowdelay_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ldm=$TEMP; fi
    TEMP=$(echo $f | sed -n '/lowdelay_P_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ldPm=$TEMP; fi
    TEMP=$(echo $f | sed -n '/randomaccess_main/p' | sed -n '/frms\./p'); if [ ${#TEMP} -ne 0 ]; then BASE_ram=$TEMP; fi
  done

  for f in *.dat
  # Find the .dats with changed params and plot rdcurves using their corresponding default / original .dat. Each rdcurve will 
  # have a unique changed parameter curve. All rdcurves will have the default / original rdcurve for comparison
  do
    TEMP=$(echo $f | sed -n '/lowdelay_main/p' | sed -n '/frms\./!p'); if [ ${#TEMP} -ne 0 ]; then gnuplot_rd2 $BASE_ldm $TEMP; fi
    TEMP=$(echo $f | sed -n '/lowdelay_P_main/p' | sed -n '/frms\./!p'); if [ ${#TEMP} -ne 0 ]; then gnuplot_rd2 $BASE_ldPm $TEMP; fi
    TEMP=$(echo $f | sed -n '/randomaccess_main/p' | sed -n '/frms\./!p'); if [ ${#TEMP} -ne 0 ]; then gnuplot_rd2 $BASE_ram $TEMP; fi
  done
}

function hevc_run {
  local CONFIGFILE=$1
  local HMBASENAME=$2
  local PSNR_RET=$3
  local INPUTVID=$4
  local BITRATE=$5
  local WIDTH=$6
  local HEIGHT=$7
  local FRAMERATE=$8
  local NOFRAMES2ENC=$9
  local XTRAPARAMS=${10} # a string containing all the adjusted parameters
 
  ./TAppEncoderStatic \
    -c $CONFIGFILE \
    --RateCtrl=1 \
    --TargetBitrate=$BITRATE \
    $XTRAPARAMS \
    -i $INPUTVID \
    -wdt $WIDTH \
    -hgt $HEIGHT \
    -fr $FRAMERATE \
    -f $NOFRAMES2ENC \
    2>&1 | tee $HMBASENAME"_"$BITRATE"kbps.txt"
  mv rec.yuv $HMBASENAME"_"$BITRATE"kbps.yuv"
  mv str.bin $HMBASENAME"_"$BITRATE"kbps.bin"

  # Print from SUMMARY to EOF | print 3rd line | print 4th column
  local PSNR_GRAB=$(sed -n '/SUMMARY/,$p' $HMBASENAME"_"$BITRATE"kbps.txt" | sed -n '3'p | awk '{print $4}')
  eval $PSNR_RET=$PSNR_GRAB
}


function hevc_setup_and_run {
  local CONFIGFILE=$1
  local DATFILE_RET=$2
  local INPUTVID=$3
  local WIDTH=$4
  local HEIGHT=$5
  local FRAMERATE=$6
  local NOFRAMES2ENC=$7
  local XTRAPARAMS=$8
  local BR_START=$9
  local BR_END=${10}
  local BR_STEP=${11}

  local CONFIG_RENAME=`echo ${CONFIGFILE/.cfg} | sed 's/encoder_//g'` 
  local XTRAPARAMS_RENAME=`echo $XTRAPARAMS | tr -d "=" | sed 's/--/_/g' | sed 's/ //g'` 
  # The basename for the files produced from the HEVC HM model
  local HMBASENAME=$CONFIG_RENAME"_"$NOFRAMES2ENC"frms"$XTRAPARAMS_RENAME
  # The name for the .dat file (summarising the HM results) used by gnuplot
  local DATFILENAME=$HMBASENAME"."$BR_START"."$BR_STEP"."$BR_END

  local STARTTIME=`date`
  
  for (( BITRATE=$BR_START; BITRATE<=$BR_END; BITRATE=$BITRATE+$BR_STEP )) ;
  do 
    hevc_run $CONFIGFILE "$HMBASENAME" PSNR $INPUTVID $BITRATE $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS"
    # write data to file
    echo "$BITRATE $PSNR" >> "$DATFILENAME.dat"
  done

  ENDTIME=`date`
  echo -e -n "\n\n\nstart_time = $STARTTIME; end_time = $ENDTIME\n\n"

  eval $DATFILE_RET=$DATFILENAME".dat"
  
}

TIMER_START=$(date +%s)

#INPUTVID='/home/stewart/Videos/claire_176x144_i420_300frms.yuv'; WIDTH=176; HEIGHT=144; FRAMERATE=30; NOFRAMES2ENC=40;
#INPUTVID='/home/stewart/Videos/foreman_176x144_i420_40frms_action.yuv';WIDTH=176;HEIGHT=144;FRAMERATE=25;NOFRAMES2ENC=40;
INPUTVID='/home/stewart/Videos/foreman_352x288_i420_300frms.yuv'; WIDTH=352; HEIGHT=288; FRAMERATE=25; NOFRAMES2ENC=50;
#INPUTVID='/home/stewart/Videos/foreman_352x288_i420_50frms_action.yuv';WIDTH=352;HEIGHT=288;FRAMERATE=25;NOFRAMES2ENC=50;

BR_START=5
BR_END=605
BR_STEP=25

CONFIGFILE_ldm='encoder_lowdelay_main.cfg'
CONFIGFILE_lpm='encoder_lowdelay_P_main.cfg'
CONFIGFILE_ram='encoder_randomaccess_main.cfg'
  
XTRAPARAMS=""
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_1 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_2 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_3 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--MaxCUWidth=32 --MaxCUHeight=32 --MaxPartitionDepth=3"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_4 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_5 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_6 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--MaxCUWidth=16 --MaxCUHeight=16 --MaxPartitionDepth=2"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_4 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_5 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_6 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--MaxCUWidth=16 --MaxCUHeight=16 --MaxPartitionDepth=1"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_4 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_5 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_6 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--RDOQ=0"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_1 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_2 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_3 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--MaxCUWidth=16 --MaxCUHeight=16 --MaxPartitionDepth=2 --RDOQ=0"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_4 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_5 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_6 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
XTRAPARAMS="--MaxCUWidth=16 --MaxCUHeight=16 --MaxPartitionDepth=1 --RDOQ=0"
hevc_setup_and_run $CONFIGFILE_ldm DATFILE_4 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP
hevc_setup_and_run $CONFIGFILE_lpm DATFILE_5 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 
hevc_setup_and_run $CONFIGFILE_ram DATFILE_6 $INPUTVID $WIDTH $HEIGHT $FRAMERATE $NOFRAMES2ENC "$XTRAPARAMS" $BR_START $BR_END $BR_STEP 


#gnuplot_rd2 $DATFILE_1 $DATFILE_2
gnuplot_rd2_automate 

TIMER_END=$(date +%s)
TIMER_DIFF=$(( $TIMER_END - $TIMER_START ))
echo "It took $TIMER_DIFF seconds to run the hevc script" > "timetakentotal.txt"
