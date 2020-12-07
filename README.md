# RainFall

## About

> Rainfall is an iso of challenge slightly more complex than Snow Crash which will plunge us in the meanders of the reverse engineering and will teach us to rebuild a code and to understand its operation in order to find the flaws.

This is the second project of the Security branch at School 42 Paris

## Content

Each level folders contains these files:

- `flag`: Password for the next user
- `source`: A pseudocode of the binary to reverse
- `README`: How to find the password
- `main.sh`: Main script to execute. It connects us through ssh and executes `script.sh` on virtual machine.

## Setup VM (Ubuntu/Debian)

Requirements:

- VirtualBox
- VirtualBox Guest Addition

```shell
./scripts/createVM.sh
./scripts/startVM.sh
./scripts/installPeda.sh
```
