#! /bin/bash

if [ $# -lt 1 ]; then
  echo ""
  echo "Error: enter directory name on command line"
  echo ""
  exit 1
fi

DIR=$1

if [ -d $DIR ]; then
  # Make sure I don't overwrite original logs incase I have to do UEP.35 reboot for FTP access. 
  echo "Error: Directory $DIR exists already. Beware of UEP reboot for FTP overwriting encoder/driver logs. Exiting"
  exit 1
fi

mkdir $DIR
cp /var/log/encoder.lo* $DIR  # Copy all the encoder logs
cp /var/log/messages $DIR
cp davinci.log $DIR

TARNAME=$DIR".tar"
tar cf $TARNAME $DIR

# Sometimes getting tar errors, do a few.
# Can FTP via "promp; mput 120628.*", where 120628 is $DIR
TARNAME=$DIR".2.tar"
tar cf $TARNAME $DIR
TARNAME=$DIR".tgz"
tar czf $TARNAME $DIR
TARNAME=$DIR".2.tgz"
tar czf $TARNAME $DIR

exit 0