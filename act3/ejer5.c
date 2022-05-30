#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    char program[90];
    pid_t p;

    printf("Ingrese el nombre de un programa a ejecutar: ");
    scanf("%s%*c", program);

    while (strcmp(program, "exit"))
    {
        char path[100];
        strcpy(path, "./");
        strcat(path, program);

        p = fork();

        if (p == 0)
        {
            execlp(path, program, NULL);
        }

        wait(NULL);

        printf("Ingrese el nombre de un programa a ejecutar: ");
        scanf("%s%*c", program);
    }
}
