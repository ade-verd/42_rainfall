# Bonus2 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus2 | `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245` |
| bonus3 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute the binary

```shell
~/bonus2

~/bonus2 a

~/bonus2 a b
Hello a
```

```shell
(gdb) info functions
0x08048484  greetuser
0x08048529  main
```

See our assembly interpretation in [source file](../source.c)

---

## Sources

### Hack

### C

- [Man strncpy](https://linux.die.net/man/3/strncpy)
- [Man getenv](https://linux.die.net/man/3/getenv)
- [Man memcmp](https://linux.die.net/man/3/memcmp)
- [Man strcat](https://linux.die.net/man/3/strcat)
- [Man puts](https://linux.die.net/man/3/puts)
