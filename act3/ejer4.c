#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char program[50];

    printf("Ingrese el nombre de un programa a ejecutar: ");
    scanf("%s%*c", program);

    while (strcmp(program, "exit"))
    {
        system(program);

        printf("Ingrese el nombre de un programa a ejecutar: ");
        scanf("%s%*c", program);
    }
}
