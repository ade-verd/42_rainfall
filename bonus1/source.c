// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int n;
    char ptr[60];

    n = atoi(argv[1]); //int atoi(const char *nptr);
    if (n > 9)
        return 1;
    n *= 4;
    memcpy(ptr, argv[2], (size_t)n);  // void *memcpy(void *dest, const void *src, size_t n);
    if (n == 1464814662)              // 0x574f4c46
        execl("/bin/sh", "sh", NULL); // int execl(const char *path, const char *arg, ...);
    return 0;
}

// main
//    0x08048424 <+0>:     push   ebp
//    0x08048425 <+1>:     mov    ebp,esp
//    0x08048427 <+3>:     and    esp,0xfffffff0
//    0x0804842a <+6>:     sub    esp,0x40
//    0x0804842d <+9>:     mov    eax,DWORD PTR [ebp+0xc]           # char **arg
//    0x08048430 <+12>:    add    eax,0x4                           # argv[1]
//    0x08048433 <+15>:    mov    eax,DWORD PTR [eax]
//    0x08048435 <+17>:    mov    DWORD PTR [esp],eax
//    0x08048438 <+20>:    call   0x8048360 <atoi@plt>              # n = atoi(argv[1])
//    0x0804843d <+25>:    mov    DWORD PTR [esp+0x3c],eax
//    0x08048441 <+29>:    cmp    DWORD PTR [esp+0x3c],0x9          # cmp n, 0x9
//    0x08048446 <+34>:    jle    0x804844f <main+43>               # if n <= 9 => goto main+43
//    0x08048448 <+36>:    mov    eax,0x1
//    0x0804844d <+41>:    jmp    0x80484a3 <main+127>
//    0x0804844f <+43>:    mov    eax,DWORD PTR [esp+0x3c]          # n
//    0x08048453 <+47>:    lea    ecx,[eax*4+0x0]                   # n *= 4
//    0x0804845a <+54>:    mov    eax,DWORD PTR [ebp+0xc]           # char **argv
//    0x0804845d <+57>:    add    eax,0x8                           # argv[2]
//    0x08048460 <+60>:    mov    eax,DWORD PTR [eax]
//    0x08048462 <+62>:    mov    edx,eax                           # argv[2]
//    0x08048464 <+64>:    lea    eax,[esp+0x14]
//    0x08048468 <+68>:    mov    DWORD PTR [esp+0x8],ecx           # n
//    0x0804846c <+72>:    mov    DWORD PTR [esp+0x4],edx           # argv[2]
//    0x08048470 <+76>:    mov    DWORD PTR [esp],eax
//    0x08048473 <+79>:    call   0x8048320 <memcpy@plt>            # memcpy(ptr)
//    0x08048478 <+84>:    cmp    DWORD PTR [esp+0x3c],0x574f4c46   # cmp n, 0x574f4c46
//    0x08048480 <+92>:    jne    0x804849e <main+122>              # if n != 0x574f4c46 => main+122
//    0x08048482 <+94>:    mov    DWORD PTR [esp+0x8],0x0
//    0x0804848a <+102>:   mov    DWORD PTR [esp+0x4],0x8048580     # 0x8048580: "sh"
//    0x08048492 <+110>:   mov    DWORD PTR [esp],0x8048583         # 0x8048583: "/bin/sh"
//    0x08048499 <+117>:   call   0x8048350 <execl@plt>             # execl("/bin/sh", "sh", NULL);
//    0x0804849e <+122>:   mov    eax,0x0
//    0x080484a3 <+127>:   leave
//    0x080484a4 <+128>:   ret