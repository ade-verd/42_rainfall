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

> _Modern Linux distributions offer some mitigation techniques to make it harder to exploit software vulnerabilities reliably. Mitigations such as RELRO, NoExecute (NX), Stack Canaries, Address Space Layout Randomization (ASLR) and Position Independent Executables (PIE) have made reliably exploiting any vulnerabilities that do exist far more challenging. The checksec.sh script is designed to test what standard Linux OS and PaX security features are being used._ (Source: trapkit.de)

---

## Sources

- [Checksec.sh script](http://www.trapkit.de/tools/checksec.html)
