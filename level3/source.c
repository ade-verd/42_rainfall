// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>

unsigned int m;

void v()
{
    char buf[0x200];

    fgets(buf, 0x200, stdin);
    printf(buf);
    if (m == 0x40)
    {
        fwrite("Wait what?!\n", 0x1, 0xc, stdout);
        system("/bin/sh");
    }
}

// main
//    0x0804851a <+0>:     push   ebp
//    0x0804851b <+1>:     mov    ebp,esp
//    0x0804851d <+3>:     and    esp,0xfffffff0
//    0x08048520 <+6>:     call   0x80484a4 <v>                     # call v()
//    0x08048525 <+11>:    leave
//    0x08048526 <+12>:    ret

// v
//    0x080484a4 <+0>:     push   ebp
//    0x080484a5 <+1>:     mov    ebp,esp
//    0x080484a7 <+3>:     sub    esp,0x218
//    0x080484ad <+9>:     mov    eax,ds:0x8049860                  # 0x8049860 <stdin@@GLIBC_2.0>
//    0x080484b2 <+14>:    mov    DWORD PTR [esp+0x8],eax           # buffer starts at esp+0x8
//    0x080484b6 <+18>:    mov    DWORD PTR [esp+0x4],0x200         # fgets maxLength = 0x200 (512)
//    0x080484be <+26>:    lea    eax,[ebp-0x208]
//    0x080484c4 <+32>:    mov    DWORD PTR [esp],eax
//    0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>             # call fgets(buf, 0x200, stdin)
//    0x080484cc <+40>:    lea    eax,[ebp-0x208]
//    0x080484d2 <+46>:    mov    DWORD PTR [esp],eax
//    0x080484d5 <+49>:    call   0x8048390 <printf@plt>            # call printf(buf)
//    0x080484da <+54>:    mov    eax,ds:0x804988c
//    0x080484df <+59>:    cmp    eax,0x40                          # if $eax == 0x40 (64) ?
//    0x080484e2 <+62>:    jne    0x8048518 <v+116>                 # if != jump leave
//    0x080484e4 <+64>:    mov    eax,ds:0x8049880                  # 0x8049880 <stdout@@GLIBC_2.0>
//    0x080484e9 <+69>:    mov    edx,eax
//    0x080484eb <+71>:    mov    eax,0x8048600                     # 0x8048600:       "Wait what?!\n"
//    0x080484f0 <+76>:    mov    DWORD PTR [esp+0xc],edx
//    0x080484f4 <+80>:    mov    DWORD PTR [esp+0x8],0xc           # nmemb=12
//    0x080484fc <+88>:    mov    DWORD PTR [esp+0x4],0x1           # size=1
//    0x08048504 <+96>:    mov    DWORD PTR [esp],eax
//    0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>            # call fwrite("Wait what?!\n", 1, 12, stdout)
//    0x0804850c <+104>:   mov    DWORD PTR [esp],0x804860d         # 0x804860d:       "/bin/sh"
//    0x08048513 <+111>:   call   0x80483c0 <system@plt>            # call system("/bin/sh")
//    0x08048518 <+116>:   leave
//    0x08048519 <+117>:   ret
