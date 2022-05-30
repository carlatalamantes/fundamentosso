#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void forkEx(){
	int i;
	if(fork()==0){
		for(i=0;i<10;i++){
			printf("Soy el hijo\n");
			sleep(1);
		}
		exit(0);
	}
	wait(NULL);
	
	
	for(i=0;i<10;i++){
			printf("Soy el padre\n");
			sleep(1);
	}
	
	
	printf("Mi proceso hijo ya ha terminado\n");

}
	
	
int main(){
	forkEx();
	return 0;
}