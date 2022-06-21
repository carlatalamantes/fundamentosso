#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
* - 4 procesos que buscan números primos (buscadores)
* - 1 proceso que recibe los numeros, los guarda en una lista encadenada y los enseña en orden ascendente (mostrador)
*/


int isPrime(int n);
void buscador();
void mostrador();



int main(int argc, char *argv[])
{
	int start_range = atoi(argv[1]);
	int end_range = atoi(argv[2]);
	int i;
	
	printf("Range introduced: %d - %d\n",start_range,end_range);
	
	for(i=start_range+1;i<end_range;i++)
	{
		if(isPrime(i))
		{
			printf("%d\n",i);
		
		}
	}
}

int isPrime(int n)
{
	int isPrime=1;
	int i;
	
	for(i=2;i<=n/2;i++)
	{
		if(n%i ==0)
		{
			isPrime=0;
			break;
		}
		
	}
	return isPrime;
}
