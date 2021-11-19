#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void log_wrapper(FILE *file, char *str, char *name)
{
    char path[255];

    strcpy(path, str);
    snprintf(path + strlen(path), 255 - 1 - strlen(path), name);

    path[strcspn(path, "\n")] = 0;
    fprintf(file, "LOG: %s\n", path);
}

int main(int ac, char *av[])
{
    int8_t  c           = -1;
    int32_t dest        = -1;
    char    path[100];
    
    if (ac != 2)
    {
        printf("Usage: %s filename\n", *av);
    }

    FILE *backups_log;
    if (!(backups_log = fopen("./backups/.log", "w")))
    {
        printf("ERROR: Failed to open%s\n", "./backups/.log");
        exit(1);
    }

    log_wrapper(backups_log, "Starting back up: ", av[1]);

    FILE *source;
    if (!(source = fopen(av[1], "r")))
    {
        printf("ERROR: Failed to open %s\n", av[1]);
        exit(1);
    }

    strcpy(path, "./backups/");
    strncat(path, av[1], 100 - 1 - strlen(path));

    if ((dest = open(path, 0xc1, 0x1b0) < 0))
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", av[1]);
        exit(1);
    }

    do
    {
        write(dest, &c, 1);
		c = fgetc(source);

    } while (c != 0xff);

    log_wrapper(backups_log, "Finished back up ", av[1]);
    fclose(source);
    close(dest);

    return (0);
}