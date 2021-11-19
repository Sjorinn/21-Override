#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>

int auth(char *login, unsigned int serial)
{
    int hash;
	size_t len;

	login[strcspn(login, "\n")] = 0;
	len = strnlen(login, 32);
	
	if (len <= 5)
    {
		return (1);
	}
	if (ptrace(0, 0, 1, 0) == -1)
    {
		puts("\e[32m.---------------------------.");
		puts("\e[31m| !! TAMPERING DETECTED !!  |");
		puts("\e[32m.---------------------------.");
		return (1);
	}
	hash = ((int) (login[3])) ^ 0x1337 + 0x5eeded; 
	for (int i = 0; i < len; i++)
    {
		if (login[i] <= 31)
        {
			return (1);
        }
		int a = login[i] ^ hash;
        int b = 0x88233b2b * a;
		int c = (a - b) / 2;
        int d = (c + b) / 1024 * 0x539;
        hash += a - d;
	}
	if (hash == serial)
    {
		return (0);
    }
	else
    {
		return (1);
    }
}

int	main()
{
	int32_t	serial;
	char	login[32];

	puts("***********************************");
	puts("*\t\tlevel06\t\t  *");
	puts("***********************************");
	printf("-> Enter Login: ");
	fgets(&login, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
	printf("-> Enter Serial: ");
	scanf("%u", serial);


	if (!(auth(login, serial)))
    {
	    puts("Authenticated!");
	    system("/bin/sh");
    }
    else
    {
        return (1);
    }
	return (0);
}