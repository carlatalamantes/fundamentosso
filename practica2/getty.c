#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
    1. Login prompt
    2. Validate credentials
    3. Fork (sh)
*/

int login(char *input_user, char *input_password)
{
    FILE *file = fopen("passwd.txt", "r");
    char line[1000] = {'\0'};
    strcat(input_user, ":");
    strcat(input_user, input_password);
    int result = 0;

    if (file == NULL)
    {
        printf("\n[ERROR]: Cannot open auth file\n");
        return 0;
    }
    else
    {
        while (fgets(line, 1000, file) != NULL)
        {

            line[strlen(line)-1]='\0';
            if ((strcmp(input_user, line) == 0))
            {
                result = 1;
                break;
            }
        }

        fclose(file);
    }

    return result;
}

int main()
{
    char input_user[20];
    char input_password[20];

    int pid;

    do
    {
        printf("\n**** LOG IN ****\n");
        printf("username:");
        scanf("%s", input_user);
        printf("password:");
        scanf("%s", input_password);

        if (login(input_user, input_password) == 1)
        {
            pid = fork();
            if (pid == 0)
            {
                execlp("./sh", "sh", NULL);
            }
            wait(NULL);

            printf("\n**** LOGGING OUT ****\n");
        }

    } while (strcmp(input_user, "exit") != 0);
}
