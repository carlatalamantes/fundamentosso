#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	int pid=fork();
	if(pid==0){
		sleep(20);
		printf("CHILD executing\n");
		exit(0);
	}else{
		printf("PARENT executing\n");
		wait(NULL);
		sleep(1);
	}
	
	return 0;
}

