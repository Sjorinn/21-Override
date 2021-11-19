#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef struct s_message
{
    char        text[140];
    char        username[40];
    int         len;
} t_message;


void secret_backdoor()
{
    char        buffer[128];

    fgets(buffer, 128, stdin);
    system(buffer);
}

void set_username(t_message *s)
{
    int32_t     i;
    char        buff[128];
    
    memset(buff, 0, 128);

    puts(">: Enter your username");
    printf(">>: ");
    fgets(buff, 128, stdin);

    *(int32_t *)(buff + 140) = 0;

    for (i = 0; i <= 40 && buff[i]; i++)
    {
        s->username[i] = buff[i];
    }

    printf(">: Welcome, %s", s->username);
}

void set_msg(t_message *s)
{
    char        buff[1024];

    memset(buff, '\0', 128);

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buff, 1024, stdin);

    strncpy(s->text, buff, s->len); // 0x8c ; 140
}

void handle_msg()
{
    t_message   s;

    memset(s.username, 0, 40);
    s.len = 140;

    set_username(&s);
    set_msg(&s);
    puts(">: Msg sent!");
}

int main()
{
    puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
    handle_msg();
    return 0;
}