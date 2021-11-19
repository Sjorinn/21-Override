#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clear_stdin()
{
    int8_t c = 0;

    while (1)
    {
        c = getchar();
        if (c == '\n' || c == '\0')
        {
            return;
        }
    }
}

unsigned int get_unum(void)
{
    uint32_t i = 0;

    fflush(stdout);
    scanf("%u", &i);
    clear_stdin();

    return (i);
}

void prog_timeout()
{
    exit(1);
}

int store_number(int32_t *buffer)
{
    uint32_t index;
    uint32_t number;

    printf(" Number: ");
    number = get_unum();
    printf(" Index: ");
    index = get_unum();

    if (!(index % 3) || number >> 0x18 != 0xb7)
    {
        puts(" *** ERROR! ***");
        puts("   This index is reserved for wil!");
        puts(" *** ERROR! ***");
        return (1);
    }
    buffer[index] = number;
    return (0);
}

int read_number(int32_t *buffer)
{
    uint32_t index = 0;

    printf(" Index:");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, buffer[index]);
    return (0);
}

int main(int argc, char *argv[], char *envp[])
{
    int8_t  buffer[100] = {0};      // ESP + 0x24
    int8_t  **av = argv;             // ESP + 0x1c
    int8_t  **ep = envp;             // ESP + 0x18
    int32_t ret = 0;                 // ESP + 0x1b4
    int8_t  entry[20] = {0};         // ESP + 0x1b8

    while (*av)
    {
        memset(*av, 0, strlen(*av));
        *av++;
    }
    while (*ep)
    {
        memset(*ep, 0, strlen(*ep));
        *ep++;
    }
    puts("----------------------------------------------------\n  Welcome to wil's crappy number storage service!   \n----------------------------------------------------\n Commands:                                          \n    store - store a number into the data storage    \n    read  - read a number from the data storage     \n    quit  - exit the program                        \n----------------------------------------------------\n   wil has reserved some storage :>                 \n----------------------------------");
    while (1)
    {
        printf("Input command: ");
        fgets(entry, 0x14, stdin);
        entry[strlen(&entry) - 1] = 0;

        if (!strncmp(entry, "store", 5))
        {
            ret = store_number(buffer);
        }
        else if (!strncmp(entry, "read", 4))
        {
            ret = read_number(buffer);
        }
        else if (!strncmp(entry, "quit", 4))
        {
            return 0;
        }

        if (ret == 0)
        {
            printf(" Completed %s command successfully\n", &entry);
        }
        else
        {
            printf(" Failed to do %s command\n", &entry);
        }
        memset(&entry, 0, 0x14);
    }

    return (0);
}