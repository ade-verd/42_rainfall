// 32bit, executable stack, no stack protector
// g++ -m32 -g -z execstack -z norelro -fno-stack-protector source.cpp

#include <cstdlib>
#include <cstring>

class N
{

private:
    char annotation[104];
    int nb;

public:
    // constructor
    N(int n)
    {
        this->nb = n;
    }

    int operator+(const N &inst)
    {
        return this->nb + inst.nb;
    }

    int operator-(const N &inst)
    {
        return this->nb - inst.nb;
    }

    void *setAnnotation(char *str)
    {
        return memcpy(this->annotation, str, strlen(str));
    }
};

int main(int argc, char **argv)
{
    if (argc <= 1)
        _Exit(1);

    N *a = new N(5);
    N *b = new N(6);

    a->setAnnotation(argv[1]);
    // (...)
    // return edx(a, b);
    return (a + b); // using N::operator+(N&)
}

// _ZN1NplERS_ => N::operator+(N&) ()
//    0x0804873a <+0>:	push   ebp
//    0x0804873b <+1>:	mov    ebp,esp
//    0x0804873d <+3>:	mov    eax,DWORD PTR [ebp+0x8]                  # arg[0] => *b
//    0x08048740 <+6>:	mov    edx,DWORD PTR [eax+0x68]                 # b->nb => 6
//    0x08048743 <+9>:	mov    eax,DWORD PTR [ebp+0xc]                  # arg[1] => *a
//    0x08048746 <+12>:	mov    eax,DWORD PTR [eax+0x68]                 # a->nb => 5
//    0x08048749 <+15>:	add    eax,edx                                  # a->nb + b->nb
//    0x0804874b <+17>:	pop    ebp

// _ZN1N13setAnnotationEPc => N::setAnnotation(char*)
//    0x0804870e <+0>:	push   ebp
//    0x0804870f <+1>:	mov    ebp,esp
//    0x08048711 <+3>:	sub    esp,0x18
//    0x08048714 <+6>:	mov    eax,DWORD PTR [ebp+0xc]                  # arg str
//    0x08048717 <+9>:	mov    DWORD PTR [esp],eax
//    0x0804871a <+12>:	call   0x8048520 <strlen@plt>                   # strlen(str)
//    0x0804871f <+17>:	mov    edx,DWORD PTR [ebp+0x8]                  # => 0x804a008
//    0x08048722 <+20>:	add    edx,0x4                                  # +4 => 0x804a00c
//    0x08048725 <+23>:	mov    DWORD PTR [esp+0x8],eax                  # strlen
//    0x08048729 <+27>:	mov    eax,DWORD PTR [ebp+0xc]
//    0x0804872c <+30>:	mov    DWORD PTR [esp+0x4],eax                  # str => 0x804a00c
//    0x08048730 <+34>:	mov    DWORD PTR [esp],edx                      # this->annotation
//    0x08048733 <+37>:	call   0x8048510 <memcpy@plt>                   # memcpy(this->annotation, str, strlen(str))
//    0x08048738 <+42>:	leave
//    0x08048739 <+43>:	ret

// _ZN1NC2Ei => N::N(int)
//    0x080486f6 <+0>:	    push   ebp
//    0x080486f7 <+1>:	    mov    ebp,esp
//    0x080486f9 <+3>:	    mov    eax,DWORD PTR [ebp+0x8]              # arg[0] => &b
//    0x080486fc <+6>:	    mov    DWORD PTR [eax],0x8048848            # 0x8048848 <_ZTV1N+8>:	0x804873a <_ZN1NplERS_> => <N::operator+(N&)>
//    0x08048702 <+12>: 	mov    eax,DWORD PTR [ebp+0x8]
//    0x08048705 <+15>: 	mov    edx,DWORD PTR [ebp+0xc]              # arg[1] => 6
//    0x08048708 <+18>:	    mov    DWORD PTR [eax+0x68],edx
//    0x0804870b <+21>:	    pop    ebp
//    0x0804870c <+22>:	    ret

// main
//    0x080485f4 <+0>:     push   ebp
//    0x080485f5 <+1>:     mov    ebp,esp
//    0x080485f7 <+3>:     push   ebx
//    0x080485f8 <+4>:     and    esp,0xfffffff0
//    0x080485fb <+7>:     sub    esp,0x20
//    0x080485fe <+10>:    cmp    DWORD PTR [ebp+0x8],0x1               # cmp argc, 1
//    0x08048602 <+14>:    jg     0x8048610 <main+28>
//    0x08048604 <+16>:    mov    DWORD PTR [esp],0x1
//    0x0804860b <+23>:    call   0x80484f0 <_exit@plt>                 # _exit()
//    0x08048610 <+28>:    mov    DWORD PTR [esp],0x6c                  # Allocate 108 bytes (104 + 4)
//    0x08048617 <+35>:	   call   0x8048530 <_Znwj@plt>                 # _Znwj => operator new
//    0x0804861c <+40>:    mov    ebx,eax                               # &a => 0x804a008
//    0x0804861e <+42>:    mov    DWORD PTR [esp+0x4],0x5
//    0x08048626 <+50>:    mov    DWORD PTR [esp],ebx
//    0x08048629 <+53>:    call   0x80486f6 <_ZN1NC2Ei>                 # N *a = new N(0x5)
//    0x0804862e <+58>:    mov    DWORD PTR [esp+0x1c],ebx              # a
//    0x08048632 <+62>:    mov    DWORD PTR [esp],0x6c                  # Allocate 108 bytes
//    0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>                 # _Znwj => operator new
//    0x0804863e <+74>:    mov    ebx,eax                               # b => 0x804a078
//    0x08048640 <+76>:    mov    DWORD PTR [esp+0x4],0x6
//    0x08048648 <+84>:    mov    DWORD PTR [esp],ebx
//    0x0804864b <+87>:    call   0x80486f6 <_ZN1NC2Ei>                 # N *b = new N(0x6)
//    0x08048650 <+92>:    mov    DWORD PTR [esp+0x18],ebx              # b
//    0x08048654 <+96>:    mov    eax,DWORD PTR [esp+0x1c]              # a
//    0x08048658 <+100>:   mov    DWORD PTR [esp+0x14],eax
//    0x0804865c <+104>:   mov    eax,DWORD PTR [esp+0x18]
//    0x08048660 <+108>:   mov    DWORD PTR [esp+0x10],eax
//    0x08048664 <+112>:   mov    eax,DWORD PTR [ebp+0xc]               # **argv
//    0x08048667 <+115>:   add    eax,0x4                               # argv[1]
//    0x0804866a <+118>:   mov    eax,DWORD PTR [eax]
//    0x0804866c <+120>:   mov    DWORD PTR [esp+0x4],eax               # argv[1]
//    0x08048670 <+124>:   mov    eax,DWORD PTR [esp+0x14]              # a
//    0x08048674 <+128>:   mov    DWORD PTR [esp],eax
//    0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>   # a.setAnnotation(argv[1]);
//    0x0804867c <+136>:   mov    eax,DWORD PTR [esp+0x10]              # eax, &b
//    0x08048680 <+140>:   mov    eax,DWORD PTR [eax]
//    0x08048682 <+142>:   mov    edx,DWORD PTR [eax]                   # 0x8048848 => 0x804873a (<_ZN1NplERS_>:	push   ebp) => N::operator+(N&) ()
//    0x08048684 <+144>:   mov    eax,DWORD PTR [esp+0x14]              # eax, &a
//    0x08048688 <+148>:   mov    DWORD PTR [esp+0x4],eax
//    0x0804868c <+152>:   mov    eax,DWORD PTR [esp+0x10]
//    0x08048690 <+156>:   mov    DWORD PTR [esp],eax
//    0x08048693 <+159>:   call   edx                                   # edx(a, b)
//    0x08048695 <+161>:   mov    ebx,DWORD PTR [ebp-0x4]
//    0x08048698 <+164>:   leave
//    0x08048699 <+165>:   ret