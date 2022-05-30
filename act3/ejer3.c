#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	
	int i;
	int n = atoi(argv[1]);
		
	for(i=0;i<n;i++){
		if(fork()==0){
			printf("Proceso hijo %d  ---- hijo: %d ---- padre: %d \n",i, getpid(),getppid());
			exit(0);
		}
	}
	
	for(i=0;i<n;i++){
		wait(NULL);
	}
	printf("Fin\n");

	
	
	
	
	return 0;
}