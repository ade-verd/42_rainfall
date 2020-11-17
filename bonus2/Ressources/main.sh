#!/bin/bash

LEVEL="2"
PREV_LEVEL=$(($LEVEL-1))
NEXT_LEVEL=$(($LEVEL+1))

CURDIR=`dirname "$(readlink -f "$0")"`

USER="bonus$LEVEL"
PASS=`cat $CURDIR/../../bonus$PREV_LEVEL/flag`
NEXT_PASS=`cat $CURDIR/../flag`

if [ -z ${RF_HOST+x} ]; then read -p "VM Host: " RF_HOST; fi
if [ -z ${RF_PORT+x} ]; then read -p "VM Port: " RF_PORT; fi

# Connect to level and run the script
echo -e "$USER password is: $PASS\n"
(set -x
scp -q -P $RF_PORT $CURDIR/getenv.c $USER@$RF_HOST:/tmp/getenv.c
ssh -q -p $RF_PORT $USER@$RF_HOST 'bash' < $CURDIR/script.sh)

# Check flag password and token
echo -e "Expected flag: $NEXT_PASS"