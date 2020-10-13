#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

void run()
{
    fwrite("Good... Wait what?\n", 1, 19, stdout);
    system("/bin/sh");
}

int main(void)
{
    char buf[76];

    return gets(buf);
}

// run
//    0x08048444 <+0>:	push   %ebp
//    0x08048445 <+1>:	mov    %esp,%ebp
//    0x08048447 <+3>:	sub    $0x18,%esp
//    0x0804844a <+6>:	mov    0x80497c0,%eax               ; 0x80497c0: stdout
//    0x0804844f <+11>:	mov    %eax,%edx
//    0x08048451 <+13>:	mov    $0x8048570,%eax              ; 0x8048570: "Good... Wait what?\n"
//    0x08048456 <+18>:	mov    %edx,0xc(%esp)
//    0x0804845a <+22>:	movl   $0x13,0x8(%esp)              ; 0x13 = 19
//    0x08048462 <+30>:	movl   $0x1,0x4(%esp)               ; 0x1  = 1
//    0x0804846a <+38>:	mov    %eax,(%esp)
//    0x0804846d <+41>:	call   0x8048350 <fwrite@plt>       ; call fwrite("Good... Wait what?\n", 1, 19, stdout)
//    0x08048472 <+46>:	movl   $0x8048584,(%esp)            ; 0x8048584: "/bin/sh"
//    0x08048479 <+53>:	call   0x8048360 <system@plt>       ; call system("/bin/sh")
//    0x0804847e <+58>:	leave
//    0x0804847f <+59>:	ret

// main
//    0x08048480 <+0>:     push   %ebp
//    0x08048481 <+1>:     mov    %esp,%ebp
//    0x08048483 <+3>:     and    $0xfffffff0,%esp
//    0x08048486 <+6>:     sub    $0x50,%esp                ; 0x50 = 80
//    0x08048489 <+9>:     lea    0x10(%esp),%eax
//    0x0804848d <+13>:    mov    %eax,(%esp)
//    0x08048490 <+16>:    call   0x8048340 <gets@plt>      ; call gets
//    0x08048495 <+21>:    leave
//    0x08048496 <+22>:    ret
