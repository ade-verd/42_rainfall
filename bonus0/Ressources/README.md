# Bonus0 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus0 | `f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728` |
| bonus1 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute the binary

```shell
~/bonus0 aaa
> -
(stdin) a
> -
(stdin) b
> a b
```

There is 2 prompts, and it concatenates them.

Lets debug the binary

```shell
(gdb) info functions
0x080484b4  p
0x0804851e  pp
0x080485a4  main
```

See our assembly interpretation in [source file](../source.c)

```shell

```

Finally we can get the pass !

```shell
$ whoami
> bonus1
$ cat /home/user/bonus0/.pass
>
```

---

## Sources

### ASM

- []()

### C

- [Man puts](https://linux.die.net/man/3/puts)
- [Man read](https://linux.die.net/man/3/read)
- [Man strchr](https://linux.die.net/man/3/strchr)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man strncpy](https://linux.die.net/man/3/strncpy)
- [Man strcat](https://linux.die.net/man/3/strcat)
