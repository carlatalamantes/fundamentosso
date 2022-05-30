#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char cmd[20];

    do
    {
        printf(">");
    } while (strcmp(cmd, "exit") != 0 || strcmp(cmd, "shutdown") != 0 );
}
