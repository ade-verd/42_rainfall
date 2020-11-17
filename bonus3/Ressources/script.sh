#!/bin/bash

set -x
~/bonus3 "" << EOI
    echo "\nwhoami: \$(whoami)"
    echo "Next pass is:  \$(cat /home/user/end/.pass)\n"
EOI