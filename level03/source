#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void deuncrypt(result)
{
	char *uncrypt = "Q}|u`sfg~sf{}|a3";
	size_t len;
	size_t i;
	
	len = strlen(uncrypt);
	i = 0;
	
	while (i < len) 
    {
		uncrypt[i] = uncrypt[i] ^ result;
		i++;
	}
	if (strncmp(uncrypt, "Congratulations", 17) == 0) 
    {
    	system("/bin/sh");
	}
	else 
    {
		puts("\nInvalid Password");
	}
}

void test(int input, int nbr)
{
    int result;

	result = nbr - input;
	switch ( result )
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		return deuncrypt(result);
		break;
		case 22:
		default:
		return deuncrypt(rand());
		break;
	}
}

int main(void)
{
  int     input;

  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &input);
  test(input, 322424845);
  return (0);
}
