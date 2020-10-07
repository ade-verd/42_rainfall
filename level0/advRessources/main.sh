#!/bin/bash

LEVEL="14"

CURDIR=`dirname "$(readlink -f "$0")"`

PREV_LEVEL=`printf "%02d" $(($LEVEL-1))`
USER="level$LEVEL"
PW=`cat $CURDIR/../../level$PREV_LEVEL/flag`

if [ -z ${SNOW_HOST+x} ]; then read -p "VM Host: " SNOW_HOST; fi
if [ -z ${SNOW_PORT+x} ]; then read -p "VM Port: " SNOW_PORT; fi

# Then check flag on flag user
FLAG_LEVEL="flag$LEVEL"
FLAG_CONTENT=`cat $CURDIR/../flag`

# Connect to level and run the script
echo -e "$USER password is : $PW\n"
(set -x
ssh -t -q -p $SNOW_PORT $USER@$SNOW_HOST 'bash' < $CURDIR/script.sh)
# ssh -t -q -p $SNOW_PORT $USER@$SNOW_HOST)

# Check flag password and token
echo -e "\nExpected flag: $FLAG_CONTENT"