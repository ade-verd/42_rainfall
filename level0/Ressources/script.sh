#!/bin/bash

set -x

~/level0 423
PASS=`cat /home/user/level1/.pass`

set +x
echo "\nNext flag is:  $PASS"
