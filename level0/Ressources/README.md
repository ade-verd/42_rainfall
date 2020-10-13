# Level0 - Modify register value

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level0 | `level0`                                                           |
| level1 | `1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a` |

## Steps to resolve on VM

1. Connect to `level0` user

```shell
ssh level0@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0
```

Notice there is a binary with level1 setuid.

2. Debug level0

Execute level0

```shell
./level0
> Segmentation fault (core dumped)
```

Argh segfault. Lets debug with gdb

```shell
gdb level0
(gdb) disas main

> (...)
  0x08048ed1 <+17>:    mov    %eax,(%esp)               ; $eax contains argv[1]
  0x08048ed4 <+20>:    call   0x8049710 <atoi>          ; call atoi
  0x08048ed9 <+25>:    cmp    $0x1a7,%eax               ; compare eax with 423
  (...)
```

The segfault seems come from `atoi`
As atoi is espected one \*(char), maybe level0 takes one argument.

```shell
(gdb) set args helloworld
(gdb) break atoi
(gdb) run
(gdb) x /s $eax
> 0xbffff833:      "helloworld"
# atoi expects argv[1]

# The value 0x1a7 is expected
(gdb) print 0x1a7
> $1 = 423
```

The string expected by atoi is the first argument (argv[1]).

After atoi syscall, the program compares the register \$eax with the value 0x1a7 (423 in decimal).

Then:

```bash
(gdb) disas main
> (...) 0x08048ef8 <+56>:    call   0x8054680 <getegid>       ; call getegid
  (...) 0x08048f01 <+65>:    call   0x8054670 <geteuid>       ; call geteuid
  (...) 0x08048f21 <+97>:    call   0x8054700 <setresgid>     ; call setresgid
  (...) 0x08048f3d <+125>:   call   0x8054690 <setresuid>     ; call setresuid
  (...) 0x08048f51 <+145>:   call   0x8054640 <execv>         ; call execv
  (...)
```

Effectives GID and UID are gotten and set

A sub program is launch with `"/bin/sh"` as command, with `["/bin/sh", '\0']` as arguments.

```gdb
  0x08048eec <+44>:    mov    %eax,0x10(%esp)           ; *char[0]    $esp+0x10 = "/bin/sh"
  0x08048ef0 <+48>:    movl   $0x0,0x14(%esp)           ; *char[1]    $esp+0x14 = '\0'
```

Then We can take advantage of level1 setuid permission.

3. Get next level password

```shell
~/level0 423
$ id
> uid=2030(level1) gid=2020(level0) groups=2030(level1),100(users),2020(level0)
# Because of setuid bit, we have level1's UID

$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```

---

## Sources

- [x86 assenbly language syntax](https://en.wikipedia.org/wiki/X86_assembly_language#Syntax)
- [gdb cheat sheet](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)

- [FR - Introduction à gdb](https://beta.hackndo.com/introduction-a-gdb/)
