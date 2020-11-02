# Level6 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level6 | `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31` |
| level7 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level6

```shell
./level6
> Segmentation fault (core dumped)

./level6 a
> Nope
```

The program segfault without an argument

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x08048454  n
  0x08048468  m
  0x0804847c  main
  (...)
```

```shell
gdb-peda$ pdisas main

gdb-peda$ pdisas m

gdb-peda$ pdisas n
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
- [Exploit 101 - Format Strings](https://axcheron.github.io/exploit-101-format-strings/)

### Shell

- [Man malloc](https://linux.die.net/man/3/malloc)
- [Man puts](https://linux.die.net/man/3/puts)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man system](https://linux.die.net/man/3/system)
