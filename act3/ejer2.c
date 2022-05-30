#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char *argv[]){
	
	int i;
	int n = atoi(argv[1]);
		
	for(i=0;i<n+1;i++){
		if(fork()== 0){
			printf("%d\n",i);
			
		}else{
			wait(NULL);			
		}
		
		
	}
	
	
	return 0;
}