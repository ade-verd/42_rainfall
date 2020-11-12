#!/bin/bash

LEVEL="bonus0"
PREV_LEVEL="level9"

CURDIR=`dirname "$(readlink -f "$0")"`

USER="$LEVEL"
PASS=`cat $CURDIR/../../$PREV_LEVEL/flag`
NEXT_PASS=`cat $CURDIR/../flag`

if [ -z ${RF_HOST+x} ]; then read -p "VM Host: " RF_HOST; fi
if [ -z ${RF_PORT+x} ]; then read -p "VM Port: " RF_PORT; fi

# Connect to level and run the script
echo -e "$USER password is: $PASS\n"
(set -x
ssh -q -p $RF_PORT $USER@$RF_HOST 'bash' < $CURDIR/script.sh)

# Check flag password and token
echo -e "Expected flag: $NEXT_PASS"