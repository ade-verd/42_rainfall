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
gdb-peda$ pdisas main
gdb-peda$ pdisas m
gdb-peda$ pdisas n
```

See our assembly interpretation in [source file](../source.c)

```C
void n(void)
{
    system("/bin/cat /home/user/level7/.pass");
}

void m(char *str)
{
    puts(str);
}

int main(int argc, char **argv)
{
    char *dest;
    void *m_address;
    void *arg;

    dest = (void *)malloc(0x40);
    m_address = (void *)malloc(0x4);
    m_address = (void *)m;
    arg = argv + 4;
    strcpy(dest, arg);

    ((void (*)(void))m_address)();
}
```

It seems the function m in called through an address stored in a pointer.

As `strcpy` is vulnerable, the idea is to use the vulnerability to overwrite the pointer `m_address`

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
