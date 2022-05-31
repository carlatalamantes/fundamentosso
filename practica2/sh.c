#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    char cmd[20];
    int pid;

    do
    {
        printf(">");
        scanf("%s",cmd);
        if(strcmp(cmd,"shut")==0){
            /*
            EXIT HERE AND GETTY
            */
        }
        pid=fork();
        if(pid==0){
            execlp(cmd,cmd,NULL);
        }
        wait(NULL);
    } while (strcmp(cmd, "exit") != 0 );
}
