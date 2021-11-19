# Level03

### Exploit :

Replace

### Step 1 :

```
level03@OverRide:~$ ./level03
***********************************
*		level03		**
***********************************
Password:pass

Invalid Password
```

Program read from stdin and demand a password.

### Step 2 :

First, let's look at the functions used :

```bash
(gdb) info function
All defined functions:

Non-debugging symbols:
0x0804843c  _init
0x08048480  printf
0x08048480  printf@plt
0x08048490  fflush
0x08048490  fflush@plt
0x080484a0  getchar
0x080484a0  getchar@plt
0x080484b0  time
0x080484b0  time@plt
0x080484c0  __stack_chk_fail
0x080484c0  __stack_chk_fail@plt
0x080484d0  puts
0x080484d0  puts@plt
0x080484e0  system
0x080484e0  system@plt
0x080484f0  __gmon_start__
0x080484f0  __gmon_start__@plt
0x08048500  srand
0x08048500  srand@plt
0x08048510  __libc_start_main
0x08048510  __libc_start_main@plt
0x08048520  rand
0x08048520  rand@plt
0x08048530  __isoc99_scanf
0x08048530  __isoc99_scanf@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  clear_stdin
0x08048617  get_unum
0x0804864f  prog_timeout
0x08048660  decrypt
0x08048747  test
0x0804885a  main
0x080488f0  __libc_csu_init
0x08048960  __libc_csu_fini
0x08048962  __i686.get_pc_thunk.bx
0x08048970  __do_global_ctors_aux
0x0804899c  _fini
```

We had many interesting function(rand,...), a call to system and 3 other function (main, test, decrypt)

Let's see main function :

```bash
(gdb) disas main
Dump of assembler code for function main:
   0x0804885a <+0>:	push   ebp
   0x0804885b <+1>:	mov    ebp,esp
   0x0804885d <+3>:	and    esp,0xfffffff0
   0x08048860 <+6>:	sub    esp,0x20
   0x08048863 <+9>:	push   eax
   0x08048864 <+10>:	xor    eax,eax
   0x08048866 <+12>:	je     0x804886b <main+17>
   0x08048868 <+14>:	add    esp,0x4
   0x0804886b <+17>:	pop    eax
   0x0804886c <+18>:	mov    DWORD PTR [esp],0x0
   0x08048873 <+25>:	call   0x80484b0 <time@plt>
   0x08048878 <+30>:	mov    DWORD PTR [esp],eax
   0x0804887b <+33>:	call   0x8048500 <srand@plt>
   0x08048880 <+38>:	mov    DWORD PTR [esp],0x8048a48
   0x08048887 <+45>:	call   0x80484d0 <puts@plt>
   0x0804888c <+50>:	mov    DWORD PTR [esp],0x8048a6c
   0x08048893 <+57>:	call   0x80484d0 <puts@plt>
   0x08048898 <+62>:	mov    DWORD PTR [esp],0x8048a48
   0x0804889f <+69>:	call   0x80484d0 <puts@plt>
   0x080488a4 <+74>:	mov    eax,0x8048a7b
   0x080488a9 <+79>:	mov    DWORD PTR [esp],eax
   0x080488ac <+82>:	call   0x8048480 <printf@plt>
   0x080488b1 <+87>:	mov    eax,0x8048a85															// eax = "%d"
   0x080488b6 <+92>:	lea    edx,[esp+0x1c]
   0x080488ba <+96>:	mov    DWORD PTR [esp+0x4],edx
   0x080488be <+100>:	mov    DWORD PTR [esp],eax
   0x080488c1 <+103>:	call   0x8048530 <__isoc99_scanf@plt>
   0x080488c6 <+108>:	mov    eax,DWORD PTR [esp+0x1c]										// return of scanf, input
   0x080488ca <+112>:	mov    DWORD PTR [esp+0x4],0x1337d00d							// val : 322424845
   0x080488d2 <+120>:	mov    DWORD PTR [esp],eax												// input
   0x080488d5 <+123>:	call   0x8048747 <test>														// call test(input, 322424845)
   0x080488da <+128>:	mov    eax,0x0
   0x080488df <+133>:	leave
   0x080488e0 <+134>:	ret
End of assembler dump.
```

- Main function print prompt and request password

- It uses the scanf function to retrieve and store the input. It expects an int as input (main +87)

- After that it's a simply call to test function with input and a specific value (322424845)

  ```bash
  (gdb) p 0x1337d00d
  $1 = 322424845
  ```

Let's take a look to test function.

```bash
(gdb) disas test
Dump of assembler code for function test:
   0x08048747 <+0>:	push   ebp
   0x08048748 <+1>:	mov    ebp,esp
   0x0804874a <+3>:	sub    esp,0x28
   0x0804874d <+6>:	mov    eax,DWORD PTR [ebp+0x8]											// input
   0x08048750 <+9>:	mov    edx,DWORD PTR [ebp+0xc]											// 322424845
   0x08048753 <+12>:	mov    ecx,edx
   0x08048755 <+14>:	sub    ecx,eax																		// 322424845 - input
   0x08048757 <+16>:	mov    eax,ecx
   0x08048759 <+18>:	mov    DWORD PTR [ebp-0xc],eax										// result = 322424845 - input
   0x0804875c <+21>:	cmp    DWORD PTR [ebp-0xc],0x15										// if result > 21
   0x08048760 <+25>:	ja     0x804884a <test+259>												// Yes jump at test+259
   0x08048766 <+31>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048769 <+34>:	shl    eax,0x2
   0x0804876c <+37>:	add    eax,0x80489f0
   0x08048771 <+42>:	mov    eax,DWORD PTR [eax]
   0x08048773 <+44>:	jmp    eax
   0x08048775 <+46>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048778 <+49>:	mov    DWORD PTR [esp],eax
   0x0804877b <+52>:	call   0x8048660 <decrypt>												// call decrypt(result)
   0x08048780 <+57>:	jmp    0x8048858 <test+273>
   0x08048785 <+62>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048788 <+65>:	mov    DWORD PTR [esp],eax
   0x0804878b <+68>:	call   0x8048660 <decrypt>
   0x08048790 <+73>:	jmp    0x8048858 <test+273>
   0x08048795 <+78>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048798 <+81>:	mov    DWORD PTR [esp],eax
   0x0804879b <+84>:	call   0x8048660 <decrypt>
   0x080487a0 <+89>:	jmp    0x8048858 <test+273>
   0x080487a5 <+94>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487a8 <+97>:	mov    DWORD PTR [esp],eax
   0x080487ab <+100>:	call   0x8048660 <decrypt>
   0x080487b0 <+105>:	jmp    0x8048858 <test+273>
   0x080487b5 <+110>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487b8 <+113>:	mov    DWORD PTR [esp],eax
   0x080487bb <+116>:	call   0x8048660 <decrypt>
   0x080487c0 <+121>:	jmp    0x8048858 <test+273>
   0x080487c5 <+126>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487c8 <+129>:	mov    DWORD PTR [esp],eax
   0x080487cb <+132>:	call   0x8048660 <decrypt>
   0x080487d0 <+137>:	jmp    0x8048858 <test+273>
   0x080487d5 <+142>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487d8 <+145>:	mov    DWORD PTR [esp],eax
   0x080487db <+148>:	call   0x8048660 <decrypt>
   0x080487e0 <+153>:	jmp    0x8048858 <test+273>
   0x080487e2 <+155>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487e5 <+158>:	mov    DWORD PTR [esp],eax
   0x080487e8 <+161>:	call   0x8048660 <decrypt>
   0x080487ed <+166>:	jmp    0x8048858 <test+273>
   0x080487ef <+168>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487f2 <+171>:	mov    DWORD PTR [esp],eax
   0x080487f5 <+174>:	call   0x8048660 <decrypt>
   0x080487fa <+179>:	jmp    0x8048858 <test+273>
   0x080487fc <+181>:	mov    eax,DWORD PTR [ebp-0xc]
   0x080487ff <+184>:	mov    DWORD PTR [esp],eax
   0x08048802 <+187>:	call   0x8048660 <decrypt>
   0x08048807 <+192>:	jmp    0x8048858 <test+273>
   0x08048809 <+194>:	mov    eax,DWORD PTR [ebp-0xc]
   0x0804880c <+197>:	mov    DWORD PTR [esp],eax
   0x0804880f <+200>:	call   0x8048660 <decrypt>
   0x08048814 <+205>:	jmp    0x8048858 <test+273>
   0x08048816 <+207>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048819 <+210>:	mov    DWORD PTR [esp],eax
   0x0804881c <+213>:	call   0x8048660 <decrypt>
   0x08048821 <+218>:	jmp    0x8048858 <test+273>
   0x08048823 <+220>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048826 <+223>:	mov    DWORD PTR [esp],eax
   0x08048829 <+226>:	call   0x8048660 <decrypt>
   0x0804882e <+231>:	jmp    0x8048858 <test+273>
   0x08048830 <+233>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048833 <+236>:	mov    DWORD PTR [esp],eax
   0x08048836 <+239>:	call   0x8048660 <decrypt>
   0x0804883b <+244>:	jmp    0x8048858 <test+273>
   0x0804883d <+246>:	mov    eax,DWORD PTR [ebp-0xc]
   0x08048840 <+249>:	mov    DWORD PTR [esp],eax
   0x08048843 <+252>:	call   0x8048660 <decrypt>
   0x08048848 <+257>:	jmp    0x8048858 <test+273>
   0x0804884a <+259>:	call   0x8048520 <rand@plt>
   0x0804884f <+264>:	mov    DWORD PTR [esp],eax
   0x08048852 <+267>:	call   0x8048660 <decrypt>																// decrypt(rand)
   0x08048857 <+272>:	nop
   0x08048858 <+273>:	leave
   0x08048859 <+274>:	ret
```

- This function contain a swtich case and only only case call decrypt with "real" number

Let's see decrypt function :

```bash
(gdb) disas decrypt
Dump of assembler code for function decrypt:
   0x08048660 <+0>:	push   ebp
   0x08048661 <+1>:	mov    ebp,esp
   0x08048663 <+3>:	push   edi
   0x08048664 <+4>:	push   esi
   0x08048665 <+5>:	sub    esp,0x40
   0x08048668 <+8>:	mov    eax,gs:0x14
   0x0804866e <+14>:	mov    DWORD PTR [ebp-0xc],eax
   0x08048671 <+17>:	xor    eax,eax
   0x08048673 <+19>:	mov    DWORD PTR [ebp-0x1d],0x757c7d51										//const for decrypt
   0x0804867a <+26>:	mov    DWORD PTR [ebp-0x19],0x67667360										//const for decrypt
   0x08048681 <+33>:	mov    DWORD PTR [ebp-0x15],0x7b66737e										//const for decrypt
   0x08048688 <+40>:	mov    DWORD PTR [ebp-0x11],0x33617c7d										//const for decrypt
   0x0804868f <+47>:	mov    BYTE PTR [ebp-0xd],0x0
   0x08048693 <+51>:	push   eax
   0x08048694 <+52>:	xor    eax,eax
   0x08048696 <+54>:	je     0x804869b <decrypt+59>
   0x08048698 <+56>:	add    esp,0x4
   0x0804869b <+59>:	pop    eax
   0x0804869c <+60>:	lea    eax,[ebp-0x1d]
   0x0804869f <+63>:	mov    DWORD PTR [ebp-0x2c],0xffffffff
   0x080486a6 <+70>:	mov    edx,eax
   0x080486a8 <+72>:	mov    eax,0x0
   0x080486ad <+77>:	mov    ecx,DWORD PTR [ebp-0x2c]
   0x080486b0 <+80>:	mov    edi,edx
   0x080486b2 <+82>:	repnz scas al,BYTE PTR es:[edi]
   0x080486b4 <+84>:	mov    eax,ecx
   0x080486b6 <+86>:	not    eax
   0x080486b8 <+88>:	sub    eax,0x1
   0x080486bb <+91>:	mov    DWORD PTR [ebp-0x24],eax
   0x080486be <+94>:	mov    DWORD PTR [ebp-0x28],0x0
   0x080486c5 <+101>:	jmp    0x80486e5 <decrypt+133>
   0x080486c7 <+103>:	lea    eax,[ebp-0x1d]
   0x080486ca <+106>:	add    eax,DWORD PTR [ebp-0x28]
   0x080486cd <+109>:	movzx  eax,BYTE PTR [eax]
   0x080486d0 <+112>:	mov    edx,eax
   0x080486d2 <+114>:	mov    eax,DWORD PTR [ebp+0x8]
   0x080486d5 <+117>:	xor    eax,edx
   0x080486d7 <+119>:	mov    edx,eax
   0x080486d9 <+121>:	lea    eax,[ebp-0x1d]
   0x080486dc <+124>:	add    eax,DWORD PTR [ebp-0x28]
   0x080486df <+127>:	mov    BYTE PTR [eax],dl
   0x080486e1 <+129>:	add    DWORD PTR [ebp-0x28],0x1
   0x080486e5 <+133>:	mov    eax,DWORD PTR [ebp-0x28]
   0x080486e8 <+136>:	cmp    eax,DWORD PTR [ebp-0x24]
   0x080486eb <+139>:	jb     0x80486c7 <decrypt+103>
   0x080486ed <+141>:	lea    eax,[ebp-0x1d]
   0x080486f0 <+144>:	mov    edx,eax
   0x080486f2 <+146>:	mov    eax,0x80489c3
   0x080486f7 <+151>:	mov    ecx,0x11
   0x080486fc <+156>:	mov    esi,edx
   0x080486fe <+158>:	mov    edi,eax
   0x08048700 <+160>:	repz cmps BYTE PTR ds:[esi],BYTE PTR es:[edi]
   0x08048702 <+162>:	seta   dl
   0x08048705 <+165>:	setb   al
   0x08048708 <+168>:	mov    ecx,edx
   0x0804870a <+170>:	sub    cl,al
   0x0804870c <+172>:	mov    eax,ecx
   0x0804870e <+174>:	movsx  eax,al
   0x08048711 <+177>:	test   eax,eax												//compare decrypt input and string "Congratulations"
   0x08048713 <+179>:	jne    0x8048723 <decrypt+195>
   0x08048715 <+181>:	mov    DWORD PTR [esp],0x80489d4
   0x0804871c <+188>:	call   0x80484e0 <system@plt>														// call system("/bin/sh")
   0x08048721 <+193>:	jmp    0x804872f <decrypt+207>
   0x08048723 <+195>:	mov    DWORD PTR [esp],0x80489dc
   0x0804872a <+202>:	call   0x80484d0 <puts@plt>															// print invalid password
   0x0804872f <+207>:	mov    esi,DWORD PTR [ebp-0xc]
   0x08048732 <+210>:	xor    esi,DWORD PTR gs:0x14
   0x08048739 <+217>:	je     0x8048740 <decrypt+224>
   0x0804873b <+219>:	call   0x80484c0 <__stack_chk_fail@plt>
   0x08048740 <+224>:	add    esp,0x40
   0x08048743 <+227>:	pop    esi
   0x08048744 <+228>:	pop    edi
   0x08048745 <+229>:	pop    ebp
   0x08048746 <+230>:	ret
End of assembler dump.
```

- Function apply mask on our input and compare with the string "Congratulations". If it's true launch a shell

### Step 3 :

So we need to test the possibilities between 322424824 and 322424845 

```
level03@OverRide:~$ for i in {322424824..322424845}; do echo $i && (echo $i; cat -) | ./level03; done
322424824
***********************************
*		level03		**
***********************************
Password:
Invalid Password

322424825
***********************************
*		level03		**
***********************************
Password:
Invalid Password

322424826
***********************************
*		level03		**
***********************************
Password:
Invalid Password

322424827
***********************************
*		level03		**
***********************************
whoami
level04
cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
^CPassword:
level03@OverRide:~$
```

Level3 done !



## Source :

https://programmerall.com/article/9240664964/