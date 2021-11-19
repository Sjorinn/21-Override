#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char **argv)
{
	char	username[100];
	memset(username, 0, 12);
	char	flag[48];
	memset(flag, 0, 5);
	char	password[100];
	memset(password, 0, 12);
	int		len_read;
	FILE	*flag_file;

	flag_file = fopen("/home/users/level03/.pass", "r");
	if (!flag_file)
	{
		fwrite("ERROR: failed to open flag flag_file\n", 1, 36, stderr);
		exit(1);
	}
	len_read = fread(&flag, 1, 41, flag_file);
	flag[strcspn(password, "\n")] = 0;
	if (len_read != 41)
	{
		fwrite("ERROR: failed to read flag flag_file\n", 1, 36, stderr);
		fwrite("ERROR: failed to read flag flag_file\n", 1, 36, stderr);
		exit(1);
	}
	fclose(flag_file);

	puts("===== [ Secure Access System v1.0 ] =====");
	puts("/***************************************\\");
	puts("| You must login to access this system. |");
	puts("\\**************************************/");
	printf("--[ Username: ");
	fgets(username, 100, stdin);
	username[strcspn(username, "\n")] = 0;
	printf("--[ Password: ");
	fgets(password, 100, stdin);
	password[strcspn(password, "\n")] = 0;
	puts("*****************************************");

	if (strncmp(flag, password, 41))
	{
		printf(username);
		puts(" does not have access!");
		exit(1);
	}

	printf("Greetings, %s!\n", username);
	system("/bin/sh");
	return (0);
}
