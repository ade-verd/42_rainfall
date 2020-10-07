#!/bin/bash

set -x

exec gdb -q /bin/getflag << EOI 
# Bypass ptrace antidebugging
    catch syscall ptrace
    commands 1
    set (\$eax) = 0
    continue
    end
# Set a breakpoint before getuid
    break getuid
# Debug program and move forward one step after getuid
    run
    next
# Set 3014 in eax register, then continue normally the execution
    set (\$eax) = 0xbc6
    continue
    quit
EOI