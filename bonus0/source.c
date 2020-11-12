// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <unistd.h>
#include <string.h>

void p(char *str, char *prompt)
{
    char buf[4096];
    char *s;

    puts(prompt);          // int puts(const char *s);
    read(0, buf, 4096);    // ssize_t read(int fildes, void *buf, size_t nbyte);
    s = strchr(buf, '\n'); // char *strchr(const char *s, int c);
    *s = '\0';
    strncpy(str, buf, 20); // char *strncpy(char *dest, const char *src, size_t n);
}

void pp(char *str)
{
    char part1[20];
    char part2[20];
    int n;

    p(part1, " - ");
    p(part2, " - ");
    strcpy(str, part1); // char *strcpy(char *dest, const char *src);
    n = strlen(str);    // size_t strlen(const char *s);
    str[n] = ' ';
    strcat(str, part2); // char *strcat(char *dest, const char *src);
}

int main(int argc, char **argv)
{
    char str[42];

    pp(str);
    puts(str); // int puts(const char *s);
    return 0;
}

// p
//    0x080484b4 <+0>:     push   ebp
//    0x080484b5 <+1>:     mov    ebp,esp
//    0x080484b7 <+3>:     sub    esp,0x1018
//    0x080484bd <+9>:     mov    eax,DWORD PTR [ebp+0xc]       # prompt
//    0x080484c0 <+12>:    mov    DWORD PTR [esp],eax
//    0x080484c3 <+15>:    call   0x80483b0 <puts@plt>          # puts(prompt);
//    0x080484c8 <+20>:    mov    DWORD PTR [esp+0x8],0x1000
//    0x080484d0 <+28>:    lea    eax,[ebp-0x1008]              # (void *) 0xbfffe5a0
//    0x080484d6 <+34>:    mov    DWORD PTR [esp+0x4],eax
//    0x080484da <+38>:    mov    DWORD PTR [esp],0x0
//    0x080484e1 <+45>:    call   0x8048380 <read@plt>          # read(0, something, 0x1000);
//    0x080484e6 <+50>:    mov    DWORD PTR [esp+0x4],0xa
//    0x080484ee <+58>:    lea    eax,[ebp-0x1008]
//    0x080484f4 <+64>:    mov    DWORD PTR [esp],eax
//    0x080484f7 <+67>:    call   0x80483d0 <strchr@plt>        # s = strchr(something, 10);
//    0x080484fc <+72>:    mov    BYTE PTR [eax],0x0            # *s = '\0'
//    0x080484ff <+75>:    lea    eax,[ebp-0x1008]
//    0x08048505 <+81>:    mov    DWORD PTR [esp+0x8],0x14
//    0x0804850d <+89>:    mov    DWORD PTR [esp+0x4],eax
//    0x08048511 <+93>:    mov    eax,DWORD PTR [ebp+0x8]
//    0x08048514 <+96>:    mov    DWORD PTR [esp],eax
//    0x08048517 <+99>:    call   0x80483f0 <strncpy@plt>       # strncpy(buf, something, 20);
//    0x0804851c <+104>:   leave
//    0x0804851d <+105>:   ret

// pp
//    0x0804851e <+0>:     push   ebp
//    0x0804851f <+1>:     mov    ebp,esp
//    0x08048521 <+3>:     push   edi
//    0x08048522 <+4>:     push   ebx
//    0x08048523 <+5>:     sub    esp,0x50
//    0x08048526 <+8>:     mov    DWORD PTR [esp+0x4],0x80486a0 # 0x80486a0: " - "
//    0x0804852e <+16>:    lea    eax,[ebp-0x30]                # (void *) 0xbffff5a8 => null
//    0x08048531 <+19>:    mov    DWORD PTR [esp],eax
//    0x08048534 <+22>:    call   0x80484b4 <p>                 # p(part1, " - ");
//    0x08048539 <+27>:    mov    DWORD PTR [esp+0x4],0x80486a0 # 0x80486a0: " - "
//    0x08048541 <+35>:    lea    eax,[ebp-0x1c]                # (void *) 0xbffff5bc => 0xb7e5ec73 => 0x2381c381
//    0x08048544 <+38>:    mov    DWORD PTR [esp],eax
//    0x08048547 <+41>:    call   0x80484b4 <p>                 # p(part2, " - ");
//    0x0804854c <+46>:    lea    eax,[ebp-0x30]
//    0x0804854f <+49>:    mov    DWORD PTR [esp+0x4],eax
//    0x08048553 <+53>:    mov    eax,DWORD PTR [ebp+0x8]       # str
//    0x08048556 <+56>:    mov    DWORD PTR [esp],eax
//    0x08048559 <+59>:    call   0x80483a0 <strcpy@plt>        # strcpy(str, part1);
//    0x0804855e <+64>:    mov    ebx,0x80486a4                 # " "
//    0x08048563 <+69>:    mov    eax,DWORD PTR [ebp+0x8]       # str
//    0x08048566 <+72>:    mov    DWORD PTR [ebp-0x3c],0xffffffff
//    0x0804856d <+79>:    mov    edx,eax
//    0x0804856f <+81>:    mov    eax,0x0
//    0x08048574 <+86>:    mov    ecx,DWORD PTR [ebp-0x3c]
//    0x08048577 <+89>:    mov    edi,edx
//    0x08048579 <+91>:    repnz scas al,BYTE PTR es:[edi]      # => pseudo_strlen(str);
//    0x0804857b <+93>:    mov    eax,ecx                       # n = strlen(str)
//    0x0804857d <+95>:    not    eax                           # absolute value
//    0x0804857f <+97>:    sub    eax,0x1                       # -1
//    0x08048582 <+100>:   add    eax,DWORD PTR [ebp+0x8]       # str[n]
//    0x08048585 <+103>:   movzx  edx,WORD PTR [ebx]            # ' '
//    0x08048588 <+106>:   mov    WORD PTR [eax],dx
//    0x0804858b <+109>:   lea    eax,[ebp-0x1c]                # part2
//    0x0804858e <+112>:   mov    DWORD PTR [esp+0x4],eax
//    0x08048592 <+116>:   mov    eax,DWORD PTR [ebp+0x8]       # str
//    0x08048595 <+119>:   mov    DWORD PTR [esp],eax
//    0x08048598 <+122>:   call   0x8048390 <strcat@plt>        # strcat(str, part2);
//    0x0804859d <+127>:   add    esp,0x50
//    0x080485a0 <+130>:   pop    ebx
//    0x080485a1 <+131>:   pop    edi
//    0x080485a2 <+132>:   pop    ebp
//    0x080485a3 <+133>:   ret

// main
//    0x080485a4 <+0>:     push   ebp
//    0x080485a5 <+1>:     mov    ebp,esp
//    0x080485a7 <+3>:     and    esp,0xfffffff0
//    0x080485aa <+6>:     sub    esp,0x40
//    0x080485ad <+9>:     lea    eax,[esp+0x16]                # str
//    0x080485b1 <+13>:    mov    DWORD PTR [esp],eax
//    0x080485b4 <+16>:    call   0x804851e <pp>                # pp(str);
//    0x080485b9 <+21>:    lea    eax,[esp+0x16]
//    0x080485bd <+25>:    mov    DWORD PTR [esp],eax
//    0x080485c0 <+28>:    call   0x80483b0 <puts@plt>          # puts(str);
//    0x080485c5 <+33>:    mov    eax,0x0
//    0x080485ca <+38>:    leave
//    0x080485cb <+39>:    ret
