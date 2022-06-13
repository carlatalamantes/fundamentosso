#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


void printchar(char c);
void P();
void Q();

int main()
{
	int pid;
	
	srand(getpid());
		
	pid = fork();
	if(pid==0)
		P();

	pid = fork();
	if(pid==0)
		Q();
	
	wait(NULL);
	wait(NULL);
	printf("\n");

}


void P() 
{
	printchar('A');
	printchar('B');
	printchar('C');
	exit(0);
}

void Q() 
{
	printchar('D');
	printchar('E');
	exit(0);
}


void printchar(char c)
{
	usleep(rand()%1000);
	printf("%c",c);
	fflush(stdout);
	usleep(rand()%1000);
}