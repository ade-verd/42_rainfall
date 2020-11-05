# Level8 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level8 | `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9` |
| level9 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level8

```shell
./level8
> (nil), (nil)
a
> (nil), (nil)
b
(...)
```

Lets debug the binary

```shell
(gdb) info functions
0x08048564  main
```

See our assembly interpretation in [source file](../source.c)

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)
- [FR - Buffer overflow](https://beta.hackndo.com/buffer-overflow/)

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man free](https://linux.die.net/man/3/free)
- [Man fwrite](https://linux.die.net/man/3/fwrite)
- [Man malloc](https://linux.die.net/man/3/malloc)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man strdup](https://linux.die.net/man/3/strdup)
- [Man system](https://linux.die.net/man/3/system)
