# Level0

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |          |
| ------ | -------- |
| level0 | `level0` |
| flag0  | ``       |
| token  | ``       |

## Steps to resolve on VM

1. Connect to `level0` user

```shell
ssh level0@$SNOW_HOST -p 4242
# Enter the password

> GCC stack protector support:            Enabled
  Strict user copy checks:                Disabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
  grsecurity / PaX: No GRKERNSEC
  Kernel Heap Hardening: No KERNHEAP
  System-wide ASLR (kernel.randomize_va_space): Off (Setting: 0)
  RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
  No RELRO        No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   /home/user/level0/level0
```

As we can see, some scripts are executed at connection.
We can see in `.bashrc` the following command: `checksec --file level0`

## Notes

### Checksec

> _Modern Linux distributions offer some mitigation techniques to make it harder to exploit software vulnerabilities reliably. Mitigations such as RELRO, NoExecute (NX), Stack Canaries, Address Space Layout Randomization (ASLR) and Position Independent Executables (PIE) have made reliably exploiting any vulnerabilities that do exist far more challenging. The checksec.sh script is designed to test what standard Linux OS and PaX security features are being used._ (Source: trapkit.de)[http://www.trapkit.de/tools/checksec.html]

---

### System Protections

#### **RELRO - Relocation Read-Only**

Relocation Read-Only (or RELRO) is a security measure which makes some binary sections read-only.
[Source](https://ctf101.org/binary-exploitation/relocation-read-only/)

---

#### **Stack Canary**

Stack canaries, named for their analogy to a canary in a coal mine, are used to detect a stack buffer overflow before execution of malicious code can occur.
[Source](https://en.wikipedia.org/wiki/Stack_buffer_overflow#Stack_canaries)

---

#### **NX - No execution bit**

The NX bit (no-execute) is a technology used in CPUs to segregate areas of memory for use by either storage of processor instructions (code) or for storage of data, a feature normally only found in Harvard architecture processors.
[Source](https://en.wikipedia.org/wiki/NX_Bit)

---

#### **PIE - Position Independant Executables**

In computing, position-independent code (PIC) or position-independent executable (PIE) is a body of machine code that, being placed somewhere in the primary memory, executes properly regardless of its absolute address. PIC is commonly used for shared libraries, so that the same library code can be loaded in a location in each program address space where it will not overlap any other uses of memory (for example, other shared libraries).
[Source](https://en.wikipedia.org/wiki/Position-independent_code)

---

#### **RPath (Run-time seach path)**

In computing, rpath designates the run-time search path hard-coded in an executable file or library. Dynamic linking loaders use the rpath to find required libraries.<br />Specifically, it encodes a path to shared libraries into the header of an executable (or another shared library). This RPATH header value (so named in the Executable and Linkable Format header standards) may either override or supplement the system default dynamic linking search paths.
[Source](https://en.wikipedia.org/wiki/Rpath)

---

#### **RunPath**

The only difference between rpath and runpath is the order they are searched in. Specifically, their relation to LD_LIBRARY_PATH - rpath is searched in before LD_LIBRARY_PATH while runpath is searched in after. The meaning of this is that rpath cannot be changed dynamically with environment variables while runpath can.
[Source](https://amir.rachum.com/blog/2016/09/17/shared-libraries/)

---

## Sources

- [Checksec.sh script](http://www.trapkit.de/tools/checksec.html)
- [NX (No Execution) Bit](https://en.wikipedia.org/wiki/NX_Bit)
- [Stack buffer overflow](https://en.wikipedia.org/wiki/Stack_buffer_overflow)
- [Stack Canaries](https://en.wikipedia.org/wiki/Stack_buffer_overflow#Stack_canaries)
- [PIC/PIE - Position independent code](https://en.wikipedia.org/wiki/Position-independent_code)
- [Rpath](https://en.wikipedia.org/wiki/Rpath)
- [Shared Libraries](https://amir.rachum.com/blog/2016/09/17/shared-libraries/)

- [FR - Le monde du Kernel - Partie 1](https://beta.hackndo.com/le-monde-du-kernel/)
- [FR - Le monde du Kernel - Partie 2 - Les failles](https://beta.hackndo.com/les-failles-kernel/)
- [FR - Le monde du Kerner - Partie 3 - Rappel d'architecture](https://beta.hackndo.com/rappels-d-architecture/)
- [FR - Technique du Canari : Bypass](https://beta.hackndo.com/technique-du-canari-bypass/)
- [FR - Introduction à gdb](https://beta.hackndo.com/introduction-a-gdb/)
