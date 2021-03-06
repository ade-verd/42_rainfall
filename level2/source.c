// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void p(void)
{
    char buf[64];
    void *ret_address;

    fflush(stdout);
    gets(buf);
    ret_address = __builtin_return_address(0);
    // printf("__builtin_return_address(0): %p\n", ret_address);
    if (((unsigned int)ret_address & 0xb0000000) == 0xb0000000)
    {
        printf("(%p\n)", ret_address);
        exit(1);
    }
    puts(buf);
    strdup(buf);
}

int main(void)
{
    p();
    return 0;
}

// main
//    0x0804853f <+0>:     push   %ebp
//    0x08048540 <+1>:     mov    %esp,%ebp
//    0x08048542 <+3>:     and    $0xfffffff0,%esp
//    0x08048545 <+6>:     call   0x80484d4 <p>             # call p()
//    0x0804854a <+11>:    leave
//    0x0804854b <+12>:    ret

// p
//    0x080484d4 <+0>:     push   %ebp
//    0x080484d5 <+1>:     mov    %esp,%ebp
//    0x080484d7 <+3>:     sub    $0x68,%esp
//    0x080484da <+6>:     mov    0x8049860,%eax            # 0x8049860 <stdout@@GLIBC_2.0>:   ""
//    0x080484df <+11>:    mov    %eax,(%esp)
//    0x080484e2 <+14>:    call   0x80483b0 <fflush@plt>    # call fflush(stdout)
//    0x080484e7 <+19>:    lea    -0x4c(%ebp),%eax          # buffer start at -0x4c(%ebp) => -76
//    0x080484ea <+22>:    mov    %eax,(%esp)
//    0x080484ed <+25>:    call   0x80483c0 <gets@plt>      # call gets(buff)
//    0x080484f2 <+30>:    mov    0x4(%ebp),%eax            # buf + 80 => $eax     (get return address)
//    0x080484f5 <+33>:    mov    %eax,-0xc(%ebp)           # $eax     => buf + 64 (???)
//    0x080484f8 <+36>:    mov    -0xc(%ebp),%eax           # buf + 64 => $eax     (???)
//    0x080484fb <+39>:    and    $0xb0000000,%eax          # $eax = $0xb0000000 & $eax
//    0x08048500 <+44>:    cmp    $0xb0000000,%eax          # compare $0xb0000000 and $eax
//    0x08048505 <+49>:    jne    0x8048527 <p+83>          # if != jump to *p+83
//    0x08048507 <+51>:    mov    $0x8048620,%eax           # 0x8048620: "(%p)\n"
//    0x0804850c <+56>:    mov    -0xc(%ebp),%edx
//    0x0804850f <+59>:    mov    %edx,0x4(%esp)
//    0x08048513 <+63>:    mov    %eax,(%esp)
//    0x08048516 <+66>:    call   0x80483a0 <printf@plt>    # call printf("(%p)\n", ptr)
//    0x0804851b <+71>:    movl   $0x1,(%esp)
//    0x08048522 <+78>:    call   0x80483d0 <_exit@plt>     # call _exit(1)
//    0x08048527 <+83>:    lea    -0x4c(%ebp),%eax
//    0x0804852a <+86>:    mov    %eax,(%esp)
//    0x0804852d <+89>:    call   0x80483f0 <puts@plt>      # call puts(buf)
//    0x08048532 <+94>:    lea    -0x4c(%ebp),%eax
//    0x08048535 <+97>:    mov    %eax,(%esp)
//    0x08048538 <+100>:   call   0x80483e0 <strdup@plt>    # call strdup(buf)
//    0x0804853d <+105>:   leave
//    0x0804853e <+106>:   ret