#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

/*
    1. Login prompt
    2. Validate credentials
    3. Fork (sh)
*/

bool login(char *input_user, char *input_password)
{
    FILE *file = fopen("passwd.txt", "r");
    char line[256];
    int i;
    char *token, *str;
    char user[20];
    char password[20];

    if (file == NULL)
    {
        printf("ERROR: Cannot open auth file\n");
        return false;
    }
    else
    {
        while (fgets(line, sizeof(line), file))
        {
            for (i = 0, str = line;; i++, str = NULL)
            {
                token = strtok(str, ":");
                if (token == NULL)
                {
                    break;
                }
                else if (i == 0)
                {
                    strcpy(user, token);
                }
                else if (i == 1)
                {
                    strcpy(password, token);
                }
            }
            
            printf("INPUT %s - %s\n", input_user, input_password);

            printf("REAL %s - %s\n", user, password);

            if ((strcmp(input_user, user) == 0) && (strcmp(input_password, password) == 0))
            {

                printf("SOAISJIAOSDJAOISDJAOIS\n");
            }
        }

        fclose(file);
    }
}

int main()
{
    char input_user[20];
    char input_password[20];

    int pid;

    do
    {
        printf("\n**** LOGIN ****\n");
        printf("username:");
        scanf("%s", input_user);
        printf("password:");
        scanf("%s", input_password);
        login(input_user, input_password);

        /* pid=fork();
        if(pid==0){
            execlp(cmd,cmd,NULL);
        }
        wait(NULL); */
    } while (strcmp(input_user, "exit") != 0);
}
