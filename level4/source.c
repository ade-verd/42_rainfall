// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>

unsigned int m = 0;

void p(char *buf)
{
    printf(buf);
}

void n()
{
    char buf[0x200];

    fgets(buf, 0x200, stdin);
    p(buf);
    if (m == 0x1025544)
    {
        system("/bin/cat /home/user/level5/.pass");
    }
}

int main(void)
{
    n();
    return 0;
}

// main
//    0x080484a7 <+0>:     push   ebp
//    0x080484a8 <+1>:     mov    ebp,esp
//    0x080484aa <+3>:     and    esp,0xfffffff0
//    0x080484ad <+6>:     call   0x8048457 <n>                 # call n()
//    0x080484b2 <+11>:    leave
//    0x080484b3 <+12>:    ret

// n
//    0x08048457 <+0>:     push   ebp
//    0x08048458 <+1>:     mov    ebp,esp
//    0x0804845a <+3>:     sub    esp,0x218
//    0x08048460 <+9>:     mov    eax,ds:0x8049804              # 0x8049804 <stdin@@GLIBC_2.0>
//    0x08048465 <+14>:    mov    DWORD PTR [esp+0x8],eax
//    0x08048469 <+18>:    mov    DWORD PTR [esp+0x4],0x200     # 0x200
//    0x08048471 <+26>:    lea    eax,[ebp-0x208]
//    0x08048477 <+32>:    mov    DWORD PTR [esp],eax
//    0x0804847a <+35>:    call   0x8048350 <fgets@plt>         # call fgets(buf, 0x200, stdin)
//    0x0804847f <+40>:    lea    eax,[ebp-0x208]
//    0x08048485 <+46>:    mov    DWORD PTR [esp],eax
//    0x08048488 <+49>:    call   0x8048444 <p>                 # call p()
//    0x0804848d <+54>:    mov    eax,ds:0x8049810              # 0x8049810 <m>:  0x00000000
//    0x08048492 <+59>:    cmp    eax,0x1025544                 # 0x1025544 = 16930116
//    0x08048497 <+64>:    jne    0x80484a5 <n+78>
//    0x08048499 <+66>:    mov    DWORD PTR [esp],0x8048590     # 0x8048590: "/bin/cat /home/user/level5/.pass"
//    0x080484a0 <+73>:    call   0x8048360 <system@plt>        # call system()
//    0x080484a5 <+78>:    leave
//    0x080484a6 <+79>:    ret

// p
//    0x08048444 <+0>:     push   ebp
//    0x08048445 <+1>:     mov    ebp,esp
//    0x08048447 <+3>:     sub    esp,0x18
//    0x0804844a <+6>:     mov    eax,DWORD PTR [ebp+0x8]
//    0x0804844d <+9>:     mov    DWORD PTR [esp],eax
//    0x08048450 <+12>:    call   0x8048340 <printf@plt>        # call printf()
//    0x08048455 <+17>:    leave
//    0x08048456 <+18>:    ret