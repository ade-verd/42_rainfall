# Level9 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level9 | `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a` |
| bonus0 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level8

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

```shell
gdb-peda$ pset arg cyclic_pattern(200)
gdb-peda$ run
gdb-peda$ pattern search
> Registers contain pattern buffer:
  EAX+0 found at offset: 108
```

Finally we can get the pass !

```shell
$ whoami
> bonus0
$ cat /home/user/bonus0/.pass
>
```

---

## Sources

### ASM

- [What is operator new(unsigned int)? - \_Znwj](https://reverseengineering.stackexchange.com/questions/4402/what-is-operator-newunsigned-int)

### CPP

- [Man \_Exit](https://en.cppreference.com/w/cpp/utility/program/_Exit)
- [Man memcpy](http://www.cplusplus.com/reference/cstring/memcpy/)
