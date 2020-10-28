// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void o(void)
{
    system("/bin/sh");
    _exit(1);
}

void n(void)
{
    char buf[0x200];

    fgets(buf, 0x200, stdin);
    printf(buf);
    exit(1);
}

int main(void)
{
    n();
}

// main
//    0x08048504 <+0>:     push   ebp
//    0x08048505 <+1>:     mov    ebp,esp
//    0x08048507 <+3>:     and    esp,0xfffffff0
//    0x0804850a <+6>:     call   0x80484c2 <n>                     # call n()
//    0x0804850f <+11>:    leave
//    0x08048510 <+12>:    ret

// n
//    0x080484c2 <+0>:     push   ebp
//    0x080484c3 <+1>:     mov    ebp,esp
//    0x080484c5 <+3>:     sub    esp,0x218
//    0x080484cb <+9>:     mov    eax,ds:0x8049848                  # 0x8049848 <stdin@@GLIBC_2.0>
//    0x080484d0 <+14>:    mov    DWORD PTR [esp+0x8],eax
//    0x080484d4 <+18>:    mov    DWORD PTR [esp+0x4],0x200
//    0x080484dc <+26>:    lea    eax,[ebp-0x208]
//    0x080484e2 <+32>:    mov    DWORD PTR [esp],eax
//    0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>             # call fgets(buf, 0x200, stdin)
//    0x080484ea <+40>:    lea    eax,[ebp-0x208]
//    0x080484f0 <+46>:    mov    DWORD PTR [esp],eax
//    0x080484f3 <+49>:    call   0x8048380 <printf@plt>            # call printf(buf)
//    0x080484f8 <+54>:    mov    DWORD PTR [esp],0x1
//    0x080484ff <+61>:    call   0x80483d0 <exit@plt>              # call exit(1)

// o
//    0x080484a4 <+0>:     push   ebp
//    0x080484a5 <+1>:     mov    ebp,esp
//    0x080484a7 <+3>:     sub    esp,0x18
//    0x080484aa <+6>:     mov    DWORD PTR [esp],0x80485f0         # 0x80485f0: "/bin/sh"
//    0x080484b1 <+13>:    call   0x80483b0 <system@plt>            # call system("/bin/sh")
//    0x080484b6 <+18>:    mov    DWORD PTR [esp],0x1
//    0x080484bd <+25>:    call   0x8048390 <_exit@plt>             # call _exit(1)