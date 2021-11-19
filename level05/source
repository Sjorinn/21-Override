#include <stdio.h>
#include <string.h>

int main(void)
{

    char buffer[100];
    int i;
    
    fgets(buffer, 100, stdin);
    i = 0;
    while (i < strlen(buffer))
	{
        buffer[i] = buffer[i] ^ 0x20; // tolower()
        i++;
    }
    printf(buffer);
    return (0);
}
