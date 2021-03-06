// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    FILE *fd;
    char buf[136];
    int n;

    fd = fopen("/home/user/end/.pass", "r"); // FILE *fopen(const char *path, const char *mode);
    memset(buf, '\0', 0x21);                 // void *memset(void *s, int c, size_t n);
    if (fd != NULL || argc != 2)
        return -1;
    fread(buf, 1, 0x42, fd); // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream );
    buf[0x41] = '\0';
    n = atoi(argv[1]); // int atoi(const char *nptr);
    buf[n] = '\0';
    fread(&buf[0x42], 1, 0x41, fd);   // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream );
    fclose(fd);                       // int fclose(FILE *fp);
    if (strcmp(buf, argv[1]) == 0)    // int strcmp(const char *s1, const char *s2);
        execl("/bin/sh", "sh", NULL); // int execl(const char *path, const char *arg, ...);
    puts(buf + 0x42);                 // int puts(const char *s);
    return 0;
}

// main
//    0x080484f4 <+0>:	    push   ebp
//    0x080484f5 <+1>:	    mov    ebp,esp
//    0x080484f7 <+3>:	    push   edi
//    0x080484f8 <+4>:	    push   ebx
//    0x080484f9 <+5>:	    and    esp,0xfffffff0
//    0x080484fc <+8>:	    sub    esp,0xa0                         # 160
//    0x08048502 <+14>: 	mov    edx,0x80486f0                    # 0x80486f0: "r"
//    0x08048507 <+19>: 	mov    eax,0x80486f2                    # 0x80486f2: "/home/user/end/.pass"
//    0x0804850c <+24>: 	mov    DWORD PTR [esp+0x4],edx
//    0x08048510 <+28>: 	mov    DWORD PTR [esp],eax
//    0x08048513 <+31>: 	call   0x8048410 <fopen@plt>            # fopen("/home/user/end/.pass", "r");
//    0x08048518 <+36>: 	mov    DWORD PTR [esp+0x9c],eax         # FILE* fd;
//    0x0804851f <+43>: 	lea    ebx,[esp+0x18]                   # char *buf
//    0x08048523 <+47>: 	mov    eax,0x0
//    0x08048528 <+52>: 	mov    edx,0x21
//    0x0804852d <+57>: 	mov    edi,ebx
//    0x0804852f <+59>: 	mov    ecx,edx
//    0x08048531 <+61>: 	rep stos DWORD PTR es:[edi],eax         # => memset(buf, '\0', 0x21);
//    0x08048533 <+63>: 	cmp    DWORD PTR [esp+0x9c],0x0         # cmp buf, 0
//    0x0804853b <+71>: 	je     0x8048543 <main+79>
//    0x0804853d <+73>: 	cmp    DWORD PTR [ebp+0x8],0x2
//    0x08048541 <+77>: 	je     0x804854d <main+89>
//    0x08048543 <+79>: 	mov    eax,0xfffffff
//    0x08048548 <+84>: 	jmp    0x8048615 <main+289>
//    0x0804854d <+89>: 	lea    eax,[esp+0x18]                   # buf
//    0x08048551 <+93>: 	mov    edx,DWORD PTR [esp+0x9c]         # fd
//    0x08048558 <+100>:	mov    DWORD PTR [esp+0xc],edx
//    0x0804855c <+104>:	mov    DWORD PTR [esp+0x8],0x42
//    0x08048564 <+112>:	mov    DWORD PTR [esp+0x4],0x1
//    0x0804856c <+120>:	mov    DWORD PTR [esp],eax
//    0x0804856f <+123>:	call   0x80483d0 <fread@plt>            # fread(buf, 0x1, 0x42, fd);
//    0x08048574 <+128>:	mov    BYTE PTR [esp+0x59],0x0          # buf[41] = '\0'
//    0x08048579 <+133>:	mov    eax,DWORD PTR [ebp+0xc]
//    0x0804857c <+136>:	add    eax,0x4                          # argv[1]
//    0x0804857f <+139>:	mov    eax,DWORD PTR [eax]
//    0x08048581 <+141>:	mov    DWORD PTR [esp],eax
//    0x08048584 <+144>:	call   0x8048430 <atoi@plt>             # atoi(argv[1]);
//    0x08048589 <+149>:	mov    BYTE PTR [esp+eax*1+0x18],0x0    # buf[n] = '\0'
//    0x0804858e <+154>:	lea    eax,[esp+0x18]                   # &buf[0]
//    0x08048592 <+158>:	lea    edx,[eax+0x42]                   # &buf[0x42]
//    0x08048595 <+161>:	mov    eax,DWORD PTR [esp+0x9c]         # fd
//    0x0804859c <+168>:	mov    DWORD PTR [esp+0xc],eax          # &buf[0]
//    0x080485a0 <+172>:	mov    DWORD PTR [esp+0x8],0x41
//    0x080485a8 <+180>:	mov    DWORD PTR [esp+0x4],0x1
//    0x080485b0 <+188>:	mov    DWORD PTR [esp],edx              # &buf[0x42]
//    0x080485b3 <+191>:	call   0x80483d0 <fread@plt>            # fread(&buf[0x42], 1, 0x41, fd);
//    0x080485b8 <+196>:	mov    eax,DWORD PTR [esp+0x9c]         # fd
//    0x080485bf <+203>:	mov    DWORD PTR [esp],eax
//    0x080485c2 <+206>:	call   0x80483c0 <fclose@plt>           # fclose(fd);
//    0x080485c7 <+211>:	mov    eax,DWORD PTR [ebp+0xc]
//    0x080485ca <+214>:	add    eax,0x4                          # argv[1]
//    0x080485cd <+217>:	mov    eax,DWORD PTR [eax]
//    0x080485cf <+219>:	mov    DWORD PTR [esp+0x4],eax
//    0x080485d3 <+223>:	lea    eax,[esp+0x18]                   # buf
//    0x080485d7 <+227>:	mov    DWORD PTR [esp],eax
//    0x080485da <+230>:	call   0x80483b0 <strcmp@plt>           # strcmp(buf, argv[1]);
//    0x080485df <+235>:	test   eax,eax                          # cmp eax, 0
//    0x080485e1 <+237>:	jne    0x8048601 <main+269>
//    0x080485e3 <+239>:	mov    DWORD PTR [esp+0x8],0x0
//    0x080485eb <+247>:	mov    DWORD PTR [esp+0x4],0x8048707    # 0x8048707: "sh"
//    0x080485f3 <+255>:	mov    DWORD PTR [esp],0x804870a        # 0x804870a: "/bin/sh"
//    0x080485fa <+262>:	call   0x8048420 <execl@plt>            # execl("/bin/sh", "sh", 0);
//    0x080485ff <+267>:	jmp    0x8048610 <main+284>
//    0x08048601 <+269>:	lea    eax,[esp+0x18]                   # buf
//    0x08048605 <+273>:	add    eax,0x42
//    0x08048608 <+276>:	mov    DWORD PTR [esp],eax
//    0x0804860b <+279>:	call   0x80483e0 <puts@plt>             # puts(buf + 42);
//    0x08048610 <+284>:	mov    eax,0x0
//    0x08048615 <+289>:	lea    esp,[ebp-0x8]
//    0x08048618 <+292>:	pop    ebx
//    0x08048619 <+293>:	pop    edi
//    0x0804861a <+294>:	pop    ebp
//    0x0804861b <+295>:	ret