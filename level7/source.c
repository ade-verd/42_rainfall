// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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

// main
//    0x08048521 <+0>:     push   ebp
//    0x08048522 <+1>:     mov    ebp,esp
//    0x08048524 <+3>:     and    esp,0xfffffff0
//    0x08048527 <+6>:     sub    esp,0x20
//    0x0804852a <+9>:     mov    DWORD PTR [esp],0x8
//    0x08048531 <+16>:    call   0x80483f0 <malloc@plt>            # malloc(0x8)
//    0x08048536 <+21>:    mov    DWORD PTR [esp+0x1c],eax          # esp+0x1c => s_arg1
//    0x0804853a <+25>:    mov    eax,DWORD PTR [esp+0x1c]
//    0x0804853e <+29>:    mov    DWORD PTR [eax],0x1               # s_arg1->integer = 1
//    0x08048544 <+35>:    mov    DWORD PTR [esp],0x8
//    0x0804854b <+42>:    call   0x80483f0 <malloc@plt>            # malloc(0x8)
//    0x08048550 <+47>:    mov    edx,eax
//    0x08048552 <+49>:    mov    eax,DWORD PTR [esp+0x1c]
//    0x08048556 <+53>:    mov    DWORD PTR [eax+0x4],edx           # s_arg1->ptr => last malloc
//    0x08048559 <+56>:    mov    DWORD PTR [esp],0x8
//    0x08048560 <+63>:    call   0x80483f0 <malloc@plt>            # malloc(0x8)
//    0x08048565 <+68>:    mov    DWORD PTR [esp+0x18],eax          # esp+0x18 => s_arg2
//    0x08048569 <+72>:    mov    eax,DWORD PTR [esp+0x18]
//    0x0804856d <+76>:    mov    DWORD PTR [eax],0x2               # s_arg2->integer = 2
//    0x08048573 <+82>:    mov    DWORD PTR [esp],0x8
//    0x0804857a <+89>:    call   0x80483f0 <malloc@plt>            # malloc(0x8)
//    0x0804857f <+94>:    mov    edx,eax
//    0x08048581 <+96>:    mov    eax,DWORD PTR [esp+0x18]
//    0x08048585 <+100>:   mov    DWORD PTR [eax+0x4],edx           # s_arg2->ptr => last malloc
//    0x08048588 <+103>:   mov    eax,DWORD PTR [ebp+0xc]           # ebp+0xc => (char**)argv
//    0x0804858b <+106>:   add    eax,0x4                           # argv + 4 = argv[1]
//    0x0804858e <+109>:   mov    eax,DWORD PTR [eax]
//    0x08048590 <+111>:   mov    edx,eax
//    0x08048592 <+113>:   mov    eax,DWORD PTR [esp+0x1c]          # s_arg1
//    0x08048596 <+117>:   mov    eax,DWORD PTR [eax+0x4]           # s_arg1->ptr
//    0x08048599 <+120>:   mov    DWORD PTR [esp+0x4],edx           # argv[1]
//    0x0804859d <+124>:   mov    DWORD PTR [esp],eax               # s_arg1->ptr
//    0x080485a0 <+127>:   call   0x80483e0 <strcpy@plt>            # strcpy(s_arg1->ptr, argv[1])
//    0x080485a5 <+132>:   mov    eax,DWORD PTR [ebp+0xc]
//    0x080485a8 <+135>:   add    eax,0x8                           # argv + 8 = argv[2]
//    0x080485ab <+138>:   mov    eax,DWORD PTR [eax]
//    0x080485ad <+140>:   mov    edx,eax
//    0x080485af <+142>:   mov    eax,DWORD PTR [esp+0x18]          # s_arg2
//    0x080485b3 <+146>:   mov    eax,DWORD PTR [eax+0x4]           # s_arg2->ptr
//    0x080485b6 <+149>:   mov    DWORD PTR [esp+0x4],edx           # argv[2]
//    0x080485ba <+153>:   mov    DWORD PTR [esp],eax               # s_arg2->ptr
//    0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>            # strcpy(s_arg2->ptr, argv[2])
//    0x080485c2 <+161>:   mov    edx,0x80486e9                     # 0x80486e9: "r"
//    0x080485c7 <+166>:   mov    eax,0x80486eb                     # 0x80486eb: "/home/user/level8/.pass"
//    0x080485cc <+171>:   mov    DWORD PTR [esp+0x4],edx
//    0x080485d0 <+175>:   mov    DWORD PTR [esp],eax
//    0x080485d3 <+178>:   call   0x8048430 <fopen@plt>             # fopen(/home/user/level8/.pass", "r")
//    0x080485d8 <+183>:   mov    DWORD PTR [esp+0x8],eax
//    0x080485dc <+187>:   mov    DWORD PTR [esp+0x4],0x44
//    0x080485e4 <+195>:   mov    DWORD PTR [esp],0x8049960         # 0x8049960 <c>: 0x00
//    0x080485eb <+202>:   call   0x80483c0 <fgets@plt>             # fgets(c, 0x44, fopen(...))
//    0x080485f0 <+207>:   mov    DWORD PTR [esp],0x8048703         # 0x8048703: "~~"
//    0x080485f7 <+214>:   call   0x8048400 <puts@plt>              # puts("~~")
//    0x080485fc <+219>:   mov    eax,0x0
//    0x08048601 <+224>:   leave
//    0x08048602 <+225>:   ret

// m
//    0x080484f4 <+0>:     push   ebp
//    0x080484f5 <+1>:     mov    ebp,esp
//    0x080484f7 <+3>:     sub    esp,0x18
//    0x080484fa <+6>:     mov    DWORD PTR [esp],0x0
//    0x08048501 <+13>:    call   0x80483d0 <time@plt>              # sec =time(NULL)
//    0x08048506 <+18>:    mov    edx,0x80486e0                     # 0x80486e0: "%s - %d\n"
//    0x0804850b <+23>:    mov    DWORD PTR [esp+0x8],eax           # time() return
//    0x0804850f <+27>:    mov    DWORD PTR [esp+0x4],0x8049960     # 0x8049960 <c>: 0x00
//    0x08048517 <+35>:    mov    DWORD PTR [esp],edx               # format string
//    0x0804851a <+38>:    call   0x80483b0 <printf@plt>            # printf("%s - %d\n", c, sec)
//    0x0804851f <+43>:    leave
//    0x08048520 <+44>:    ret