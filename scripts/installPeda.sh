#! /bin/bash

PEDA_LOCAL_PATH="/tmp/peda"
PEDA_REMOTE_PATH="/tmp/peda"
GDBINIT_REMOTE_PATH="/tmp/.gdbinit"

if [ -z ${RF_HOST+x} ]; then read -p "VM Host: " RF_HOST; fi
if [ -z ${RF_PORT+x} ]; then read -p "VM Port: " RF_PORT; fi
if [ -z ${RF_USER+x} ]; then read -p "VM User: " RF_USER; fi

set -x

# Download PEDA
rm -Rf $PEDA_LOCAL_PATH
git clone https://github.com/longld/peda.git $PEDA_LOCAL_PATH

# copy peda folder to VM
scp -q -P $RF_PORT \
    -r $PEDA_LOCAL_PATH $RF_USER@$RF_HOST:$PEDA_REMOTE_PATH

# create .gdbinit file in /tmp folder
ssh -q -p $RF_PORT $RF_USER@$RF_HOST << EOI
    echo "source $PEDA_REMOTE_PATH/peda.py" >> $GDBINIT_REMOTE_PATH
    echo -e "\n-------\n"
    echo "Done! To use gdb-peda run: "
    echo "cd /tmp; gdb <binary>"
EOI