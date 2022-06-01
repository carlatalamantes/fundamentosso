#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[])
{   
    
    char cmd[20];
    int pid;

    do
    {
        printf(">");
        fgets(cmd, 20,stdin);
        cmd[strlen(cmd)-1]='\0';
        
        /* Array de argumentos */
        char * args[50];
        char ** next = args;

        /* Obtener el primer token */
        char * token = strtok(cmd, " ");

        /* Obtener el resto */
        while(token != NULL) {
            *next ++ = token;
            token = strtok(NULL, " ");
        }
        *next = NULL;

        if(strcmp(cmd,"shut")==0){
           int time=10;
            int gpid = atoi(argv[1]);
            //Send signal to grandparent
            kill(gpid, SIGUSR1);            
           
        }
        pid=fork();
        if(pid==0){
            if(sizeof args == 1) execlp(cmd,cmd,NULL);
            else execvp(args[0],args);
        }
        wait(NULL);
    } while (strcmp(cmd, "exit") != 0 );
}
