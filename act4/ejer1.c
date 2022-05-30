#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


	
int main(){
	int pid=fork();
	if(pid==0){
		printf("CHILD[%d] ready to kill PARENT[%d]\n",getpid(),getppid());
		kill(getpid(),SIGSEGV);
	}else{
		printf("PARENT[%d] executing\n",getpid());
		wait(0);
	}
	printf("PARENT was killed. FINAL PID[%d]\n",pid);
	
	return 0;
}