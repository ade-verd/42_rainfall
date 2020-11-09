# Level9 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level9 | `c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a` |
| bonus0 | ``                                                                 |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level8

```shell
./level9
```

Nothing happened

Lets debug the binary

```shell
(gdb) info functions
0x080485f4  main
0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)

```

See our assembly interpretation in [source file](../source.c)

```shell

```

Finally we can get the pass !

```shell
$ whoami
> bonus0
$ cat /home/user/bonus0/.pass
>
```

---

## Sources

### ASM

- [What do the assembly instructions 'seta' and 'setb' do after repz cmpsb?](https://stackoverflow.com/questions/44630262/what-do-the-assembly-instructions-seta-and-setb-do-after-repz-cmpsb/44630741)
- [REPNZ SCAS Assembly Instruction Specifics](https://stackoverflow.com/questions/26783797/repnz-scas-assembly-instruction-specifics)
- [x86 Registers](https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html)
- [x86 Instruction Set Reference - REP/REPE/REPZ/REPNE/REPNZ](https://c9x.me/x86/html/file_module_x86_id_279.html)

### CPP

- [The Secret Life of C++: Objects - \_Znwj](http://web.mit.edu/tibbetts/Public/inside-c/www/objects.html)
- [Man \_Exit](https://en.cppreference.com/w/cpp/utility/program/_Exit)
