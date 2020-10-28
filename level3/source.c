// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>

void v(void)
{
    char buf[200];
    int *p = (int *)0x8049880; // manually assign address

    *p = 0;

    fgets(buf, 200, stdin);
    printf(buf);

    if (*p == 64)
    {
        fwrite("Wait what?!\n", 1, 12, stdout);
        system("/bin/sh");
    }
}

int main(void)
{
    v();
    return 0;
}

// main
//    0x0804851a <+0>:     push   %ebp
//    0x0804851b <+1>:     mov    %esp,%ebp
//    0x0804851d <+3>:     and    $0xfffffff0,%esp
//    0x08048520 <+6>:     call   0x80484a4 <v>             # call v()
//    0x08048525 <+11>:    leave
//    0x08048526 <+12>:    ret

// v
//    0x080484a4 <+0>:     push   %ebp
//    0x080484a5 <+1>:     mov    %esp,%ebp
//    0x080484a7 <+3>:     sub    $0x218,%esp
//    0x080484ad <+9>:     mov    0x8049860,%eax
//    0x080484b2 <+14>:    mov    %eax,0x8(%esp)
//    0x080484b6 <+18>:    movl   $0x200,0x4(%esp)          # buffer size of 200
//    0x080484be <+26>:    lea    -0x208(%ebp),%eax         # buffer start at -0x208(%ebp)
//    0x080484c4 <+32>:    mov    %eax,(%esp)
//    0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>     # call fgets() but protected from buffer overflow
//    0x080484cc <+40>:    lea    -0x208(%ebp),%eax
//    0x080484d2 <+46>:    mov    %eax,(%esp)               # only 1 argument given to printf (only 1 esp pointer used) => bad implementation for printf, exploit that !
//    0x080484d5 <+49>:    call   0x8048390 <printf@plt>    # call printf()
//    0x080484da <+54>:    mov    0x804988c,%eax
//    0x080484df <+59>:    cmp    $0x40,%eax                # compare 0x40 and eax ( which is 0x0)
//    0x080484e2 <+62>:    jne    0x8048518 <v+116>         # if != jump to *v+116
//    0x080484e4 <+64>:    mov    0x8049880,%eax            # 0x8049880:       stdout
//    0x080484e9 <+69>:    mov    %eax,%edx
//    0x080484eb <+71>:    mov    $0x8048600,%eax           # 0x8048600:       "Wait what?!\n"
//    0x080484f0 <+76>:    mov    %edx,0xc(%esp)
//    0x080484f4 <+80>:    movl   $0xc,0x8(%esp)            # 0xc = 12
//    0x080484fc <+88>:    movl   $0x1,0x4(%esp)            # 0x1 = 1
//    0x08048504 <+96>:    mov    %eax,(%esp)
//    0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>    # call fwrite("Wait what?!\n", 1, 12, stdout)
//    0x0804850c <+104>:   movl   $0x804860d,(%esp)         # 0x804860d:       "/bin/sh"
//    0x08048513 <+111>:   call   0x80483c0 <system@plt>    # call system("/bin/sh") which is our subshell on the level4
//    0x08048518 <+116>:   leave
//    0x08048519 <+117>:   ret