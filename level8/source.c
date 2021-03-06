// 32bit, executable stack, no stack protector
// gcc -m32 -g -z execstack -z norelro -fno-stack-protector source.c

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

char *auth = NULL;
char *service = NULL;

int main(int argc, char **argv)
{
    char buf[0x80];

    while (1)
    {
        printf("%p, %p \n", auth, service);
        if (!fgets(buf, 0x80, stdin)) // 128
            return 0;
        if (strncmp(buf, "auth ", 5) == 0)
        {
            auth = malloc(4);
            if (strlen(buf + 5) < (0x1e + 0x1)) // 31
                strcpy(auth, buf + 5);
        }
        if (strncmp(buf, "reset", 5) == 0)
            free(auth);
        if (strncmp(buf, "service", 6) == 0)
            service = strdup(buf + 7);
        if (strncmp(buf, "login", 5) == 0)
        {
            if (auth + 0x20 != 0) // 32
                system("/bin/sh");
            else
                fwrite("Password:\n", 0x1, 0xa, stdout);
        }
    }
}

// main
//    0x08048564 <+0>:	    push   ebp
//    0x08048565 <+1>:	    mov    ebp,esp
//    0x08048567 <+3>:	    push   edi
//    0x08048568 <+4>:	    push   esi
//    0x08048569 <+5>:	    and    esp,0xfffffff0
//    0x0804856c <+8>:	    sub    esp,0xa0
//    0x08048572 <+14>:	    jmp    0x8048575 <main+17>
//    0x08048574 <+16>:	    nop
//    0x08048575 <+17>:	    mov    ecx,DWORD PTR ds:0x8049ab0                   # 0x8049ab0 <service>: ""
//    0x0804857b <+23>:	    mov    edx,DWORD PTR ds:0x8049aac                   # 0x8049aac <auth>: ""
//    0x08048581 <+29>:	    mov    eax,0x8048810                                # 0x8048810: "%p, %p \n"
//    0x08048586 <+34>:	    mov    DWORD PTR [esp+0x8],ecx
//    0x0804858a <+38>:	    mov    DWORD PTR [esp+0x4],edx
//    0x0804858e <+42>:	    mov    DWORD PTR [esp],eax
//    0x08048591 <+45>:	    call   0x8048410 <printf@plt>                       # printf("%p, %p \n", auth, service);
//    0x08048596 <+50>:	    mov    eax,ds:0x8049a80                             # 0x8049a80 <stdin@@GLIBC_2.0>
//    0x0804859b <+55>:	    mov    DWORD PTR [esp+0x8],eax
//    0x0804859f <+59>:	    mov    DWORD PTR [esp+0x4],0x80
//    0x080485a7 <+67>:	    lea    eax,[esp+0x20]                               # esp+0x20 -> 0xbffff5d0 -> buf
//    0x080485ab <+71>:	    mov    DWORD PTR [esp],eax
//    0x080485ae <+74>:	    call   0x8048440 <fgets@plt>                        # fgets(&buf, 0x80, stdin);
//    0x080485b3 <+79>:	    test   eax,eax                                      # = cmp eax, 0
//    0x080485b5 <+81>:	    je     0x804872c <main+456>                         # if == 0 -> goto main+456
//    0x080485bb <+87>:	    lea    eax,[esp+0x20]
//    0x080485bf <+91>:	    mov    edx,eax
//    0x080485c1 <+93>:	    mov    eax,0x8048819                                # 0x8048819: "auth "
//    0x080485c6 <+98>:	    mov    ecx,0x5
//    0x080485cb <+103>:	mov    esi,edx
//    0x080485cd <+105>:	mov    edi,eax
//    0x080485cf <+107>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]       # strncmp(buf, "auth", 5) => Compare strings while the strings compare equal
//    0x080485d1 <+109>:	seta   dl                                           # Set dl to 1 if the above flag is set, or dl to 0 if not
//    0x080485d4 <+112>:	setb   al                                           # Set al to 1 if the below flag is set, or al to 0 if not
//    0x080485d7 <+115>:	mov    ecx,edx
//    0x080485d9 <+117>:	sub    cl,al
//    0x080485db <+119>:	mov    eax,ecx
//    0x080485dd <+121>:	movsx  eax,al
//    0x080485e0 <+124>:	test   eax,eax                                      # = cmp eax, 0
//    0x080485e2 <+126>:	jne    0x8048642 <main+222>                         # if (strncmp != 0) => goto main+222
//    0x080485e4 <+128>:	mov    DWORD PTR [esp],0x4
//    0x080485eb <+135>:	call   0x8048470 <malloc@plt>                       # malloc(4)
//    0x080485f0 <+140>:	mov    ds:0x8049aac,eax                             # 0x8049aac <auth> = malloc(4)
//    0x080485f5 <+145>:	mov    eax,ds:0x8049aac
//    0x080485fa <+150>:	mov    DWORD PTR [eax],0x0
//    0x08048600 <+156>:	lea    eax,[esp+0x20]                               # buf
//    0x08048604 <+160>:	add    eax,0x5                                      # buf + 5
//    0x08048607 <+163>:	mov    DWORD PTR [esp+0x1c],0xffffffff
//    0x0804860f <+171>:	mov    edx,eax
//    0x08048611 <+173>:	mov    eax,0x0
//    0x08048616 <+178>:	mov    ecx,DWORD PTR [esp+0x1c]
//    0x0804861a <+182>:	mov    edi,edx
//    0x0804861c <+184>:	repnz scas al,BYTE PTR es:[edi]                     # => strlen(buf + 5)
//    0x0804861e <+186>:	mov    eax,ecx
//    0x08048620 <+188>:	not    eax
//    0x08048622 <+190>:	sub    eax,0x1                                      # strlen - 1
//    0x08048625 <+193>:	cmp    eax,0x1e
//    0x08048628 <+196>:	ja     0x8048642 <main+222>                         # if (strlen-1 > 0x1e) => main+222
//    0x0804862a <+198>:	lea    eax,[esp+0x20]
//    0x0804862e <+202>:	lea    edx,[eax+0x5]
//    0x08048631 <+205>:	mov    eax,ds:0x8049aac                             # auth
//    0x08048636 <+210>:	mov    DWORD PTR [esp+0x4],edx
//    0x0804863a <+214>:	mov    DWORD PTR [esp],eax
//    0x0804863d <+217>:	call   0x8048460 <strcpy@plt>                       # strcpy(auth, buf + 5);
//    0x08048642 <+222>:	lea    eax,[esp+0x20]
//    0x08048646 <+226>:	mov    edx,eax
//    0x08048648 <+228>:	mov    eax,0x804881f                                # 0x804881f: "reset"
//    0x0804864d <+233>:	mov    ecx,0x5
//    0x08048652 <+238>:	mov    esi,edx
//    0x08048654 <+240>:	mov    edi,eax
//    0x08048656 <+242>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]       # => strncmp(buf, "reset", 5);
//    0x08048658 <+244>:	seta   dl
//    0x0804865b <+247>:	setb   al
//    0x0804865e <+250>:	mov    ecx,edx
//    0x08048660 <+252>:	sub    cl,al
//    0x08048662 <+254>:	mov    eax,ecx
//    0x08048664 <+256>:	movsx  eax,al
//    0x08048667 <+259>:	test   eax,eax                                      # = cmp eax, 0
//    0x08048669 <+261>:	jne    0x8048678 <main+276>                         # if (strncmp != 0) => goto main+276
//    0x0804866b <+263>:	mov    eax,ds:0x8049aac                             # 0x8049aac <auth>: ""
//    0x08048670 <+268>:	mov    DWORD PTR [esp],eax
//    0x08048673 <+271>:	call   0x8048420 <free@plt>                         # free(auth);
//    0x08048678 <+276>:	lea    eax,[esp+0x20]
//    0x0804867c <+280>:	mov    edx,eax
//    0x0804867e <+282>:	mov    eax,0x8048825                                # 0x8048825: "service"
//    0x08048683 <+287>:	mov    ecx,0x6
//    0x08048688 <+292>:	mov    esi,edx
//    0x0804868a <+294>:	mov    edi,eax
//    0x0804868c <+296>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]       # strncmp(buf, "service", 6);
//    0x0804868e <+298>:	seta   dl
//    0x08048691 <+301>:	setb   al
//    0x08048694 <+304>:	mov    ecx,edx
//    0x08048696 <+306>:	sub    cl,al
//    0x08048698 <+308>:	mov    eax,ecx
//    0x0804869a <+310>:	movsx  eax,al
//    0x0804869d <+313>:	test   eax,eax
//    0x0804869f <+315>:	jne    0x80486b5 <main+337>
//    0x080486a1 <+317>:	lea    eax,[esp+0x20]                               # buf
//    0x080486a5 <+321>:	add    eax,0x7
//    0x080486a8 <+324>:	mov    DWORD PTR [esp],eax
//    0x080486ab <+327>:	call   0x8048430 <strdup@plt>                       # strdup(buf + 7);
//    0x080486b0 <+332>:	mov    ds:0x8049ab0,eax                             # 0x8049ab0 <service>: ""
//    0x080486b5 <+337>:	lea    eax,[esp+0x20]
//    0x080486b9 <+341>:	mov    edx,eax
//    0x080486bb <+343>:	mov    eax,0x804882d                                # 0x804882d: "login"
//    0x080486c0 <+348>:	mov    ecx,0x5
//    0x080486c5 <+353>:	mov    esi,edx
//    0x080486c7 <+355>:	mov    edi,eax
//    0x080486c9 <+357>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]       # strncpm(buf, login, 5);
//    0x080486cb <+359>:	seta   dl
//    0x080486ce <+362>:	setb   al
//    0x080486d1 <+365>:	mov    ecx,edx
//    0x080486d3 <+367>:	sub    cl,al
//    0x080486d5 <+369>:	mov    eax,ecx
//    0x080486d7 <+371>:	movsx  eax,al
//    0x080486da <+374>:	test   eax,eax
//    0x080486dc <+376>:	jne    0x8048574 <main+16>
//    0x080486e2 <+382>:	mov    eax,ds:0x8049aac                             # 0x8049aac <auth>: ""
//    0x080486e7 <+387>:	mov    eax,DWORD PTR [eax+0x20]                     # auth + 0x20
//    0x080486ea <+390>:	test   eax,eax
//    0x080486ec <+392>:	je     0x80486ff <main+411>
//    0x080486ee <+394>:	mov    DWORD PTR [esp],0x8048833                    # 0x8048833: "/bin/sh"
//    0x080486f5 <+401>:	call   0x8048480 <system@plt>                       # system("/bin/sh");
//    0x080486fa <+406>:	jmp    0x8048574 <main+16>
//    0x080486ff <+411>:	mov    eax,ds:0x8049aa0                             # 0x8049aa0 <stdout@@GLIBC_2.0>
//    0x08048704 <+416>:	mov    edx,eax
//    0x08048706 <+418>:	mov    eax,0x804883b                                # 0x804883b: "Password:\n"
//    0x0804870b <+423>:	mov    DWORD PTR [esp+0xc],edx
//    0x0804870f <+427>:	mov    DWORD PTR [esp+0x8],0xa
//    0x08048717 <+435>:	mov    DWORD PTR [esp+0x4],0x1
//    0x0804871f <+443>:	mov    DWORD PTR [esp],eax
//    0x08048722 <+446>:	call   0x8048450 <fwrite@plt>                       # fwrite("Password:\n", 0x1, 0xa, stdout)
//    0x08048727 <+451>:	jmp    0x8048574 <main+16>
//    0x0804872c <+456>:	nop
//    0x0804872d <+457>:	mov    eax,0x0
//    0x08048732 <+462>:	lea    esp,[ebp-0x8]
//    0x08048735 <+465>:	pop    esi
//    0x08048736 <+466>:	pop    edi
//    0x08048737 <+467>:	pop    ebp
//    0x08048738 <+468>:	ret
