# Level2 -

## Local script usage

```shell
Usage: ./main.sh
```

## Passwords and token

|        |                                                                    |
| ------ | ------------------------------------------------------------------ |
| level2 | `53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77` |
| level3 | ``                                                                 |

## Steps to resolve on VM

1. Connect to `level2` user

```shell
ssh level2@$SNOW_HOST -p 4242
# Enter the password

ls -l
> -rwsr-s---+ 1 level3 users 5403 Mar  6  2016 level2
```

Notice there is a binary with level2 setuid.

2. Debug level2

Execute level2

```shell
./level2
(stdin) abcdef
> abcdef
```

Lets debug the binary

```shell
(gdb) info functions

> (...)
  0x080484d4  p
  0x0804853f  main
  (...)
```

---

## Sources

### Buffer overflow

- []()

### ASM explications

- []()

### Shell

- []()
