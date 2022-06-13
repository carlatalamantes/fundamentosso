#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 8
#define ITERS 1000

int cuenta=0;


void *tfunc(void *args)
{
	int nthread = *((int *) args);
	int i;
	
	printf("Hola desde el hilo %d\n",nthread);
	printf("Cuenta actual = %d\n",cuenta);
	for(i=0;i<ITERS;i++)
		cuenta++;		// mov [cuenta],REG
						// inc REG
						// mov REG,[cuenta]
}

int main()
{
	int i;
	pthread_t tid[NTHREADS];
	int args[NTHREADS];
	

	
	for(i=0;i<NTHREADS;i++)
	{
		args[i] = i;
		pthread_create(&tid[i],NULL,tfunc,&args[i]);
	}
	
	for(i=0;i<NTHREADS;i++)
		pthread_join(tid[i],NULL);
	
	
	printf("Cuenta final = %d\n",cuenta);
}