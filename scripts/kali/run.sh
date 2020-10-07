#! /bin/bash

if [ -z ${SNOW_HOST+x} ]; then read -p "VM Host: " SNOW_HOST; fi
if [ -z ${SNOW_PORT+x} ]; then read -p "VM Port: " SNOW_PORT; fi

docker run -it \
    --rm \
    --name kali \
    -e SNOW_HOST=$SNOW_HOST \
    -e SNOW_PORT=$SNOW_PORT \
    kali-light