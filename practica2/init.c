#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NPROCS 6
int pid[NPROCS];

/*
	1. Crear 6 procesos hijos
    2. Ejecutar el getty 
	NOTA: Siempre deben estar ejecutandose 6 procesos
    NOTA: Solo terminará utilizando shutdown 
    
 */

void signhandler(int signal){
	int i;
	for(i=0;i<6;i++){
		kill(pid[i],SIGKILL);
	}
	
}

int main() {
	int i;
    char s_my_pid[10];
    sprintf(s_my_pid,"%d\n ", getpid());
	signal(SIGUSR1,signhandler);

  
	for(i=0;i<NPROCS;i++){
		pid[i]=fork();

        if(pid[i] == 0 ){
            execlp("/bin/xterm","xterm","-e","./getty", s_my_pid, NULL);
        }
		
	}
	
	for(i=0;i<NPROCS;i++){
		wait(NULL);
	}
}
