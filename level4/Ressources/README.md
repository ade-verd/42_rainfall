# Level4 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level4 | `b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa` |
| level5 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

1. Connect to `level4` user

```shell
ssh level4@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level5 users 5252 Mar  6  2016 level4
```

Notice there is a binary with level4 setuid and setgid.

2. Debug level4

Execute level4

```shell
./level4
(stdin) abcdef
> abcdef
```

Seems to be like previous levels.

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x08048444  p
  0x08048457  n
  0x080484a7  main
  (...)
```

```shell
gdb-peda$ pdisas main
# (...)

gdb-peda$ pdisas p
# (...)

gdb-peda$ pdisas n
# (...)
```

---

## Sources

### Format string vulnerability

- [Memory segmentation - Cheat sheet](https://www.0x0ff.info/wp-content/uploads/2015/12/buffer-overflow-memory-segmentation-cheat-sheet.png)

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man system](https://linux.die.net/man/3/system)
