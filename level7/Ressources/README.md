# Level7 - 

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level7 | `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d` |
| level8 | `` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level7

```shell
./level7
> Segmentation fault (core dumped)

./level7 a
> Segmentation fault (core dumped)

./level7 a b
> ~~
```

The program seems take two arguments

Lets debug the binary

```shell
(gdb) info functions
0x080484f4  m
0x08048521  main
```

See our assembly interpretation in [source file](../source.c)

```C
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
- [FR - Buffer overflow](https://beta.hackndo.com/buffer-overflow/)

### Shell

- [Man malloc](https://linux.die.net/man/3/malloc)
- [Man puts](https://linux.die.net/man/3/puts)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man fopen](https://linux.die.net/man/3/fopen)
- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man time]()
- [Man printf]()
