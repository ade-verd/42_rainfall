// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

// main
//    0x0804847c <+0>:     push   ebp
//    0x0804847d <+1>:     mov    ebp,esp
//    0x0804847f <+3>:     and    esp,0xfffffff0
//    0x08048482 <+6>:     sub    esp,0x20
//    0x08048485 <+9>:     mov    DWORD PTR [esp],0x40
//    0x0804848c <+16>:    call   0x8048350 <malloc@plt>        ; malloc(0x40)
//    0x08048491 <+21>:    mov    DWORD PTR [esp+0x1c],eax      ; char* dest
//    0x08048495 <+25>:    mov    DWORD PTR [esp],0x4
//    0x0804849c <+32>:    call   0x8048350 <malloc@plt>        ; malloc(0x4)
//    0x080484a1 <+37>:    mov    DWORD PTR [esp+0x18],eax      ; void* m_address
//    0x080484a5 <+41>:    mov    edx,0x8048468                 ; 0x8048468 <m>: m-function address
//    0x080484aa <+46>:    mov    eax,DWORD PTR [esp+0x18]
//    0x080484ae <+50>:    mov    DWORD PTR [eax],edx           ; m_address = &m
//    0x080484b0 <+52>:    mov    eax,DWORD PTR [ebp+0xc]       ; (char**)argv
//    0x080484b3 <+55>:    add    eax,0x4                       ; eax = argv + 4 => argv[1]
//    0x080484b6 <+58>:    mov    eax,DWORD PTR [eax]
//    0x080484b8 <+60>:    mov    edx,eax                       ; edx = argv[1]
//    0x080484ba <+62>:    mov    eax,DWORD PTR [esp+0x1c]      ;
//    0x080484be <+66>:    mov    DWORD PTR [esp+0x4],edx       ;
//    0x080484c2 <+70>:    mov    DWORD PTR [esp],eax           ;
//    0x080484c5 <+73>:    call   0x8048340 <strcpy@plt>        ; strcpy(dest, argv[1])
//    0x080484ca <+78>:    mov    eax,DWORD PTR [esp+0x18]
//    0x080484ce <+82>:    mov    eax,DWORD PTR [eax]
//    0x080484d0 <+84>:    call   eax                           ; ((void (*)(void))m_address)();
//    0x080484d2 <+86>:    leave
//    0x080484d3 <+87>:    ret

// m
//    0x08048468 <+0>:     push   ebp
//    0x08048469 <+1>:     mov    ebp,esp
//    0x0804846b <+3>:     sub    esp,0x18
//    0x0804846e <+6>:     mov    DWORD PTR [esp],0x80485d1     ; 0x80485d1: "Nope"
//    0x08048475 <+13>:    call   0x8048360 <puts@plt>          ; puts("Nope")
//    0x0804847a <+18>:    leave
//    0x0804847b <+19>:    ret

// n
//    0x08048454 <+0>:     push   ebp
//    0x08048455 <+1>:     mov    ebp,esp
//    0x08048457 <+3>:     sub    esp,0x18
//    0x0804845a <+6>:     mov    DWORD PTR [esp],0x80485b0     ; 0x80485b0: "/bin/cat /home/user/level7/.pass"
//    0x08048461 <+13>:    call   0x8048370 <system@plt>        : system("/bin/cat /home/user/level7/.pass")
//    0x08048466 <+18>:    leave
//    0x08048467 <+19>:    ret