#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE 4000
#define INICIAL 900000000
#define FINAL 1000000000
#define NTHREADS 4

int mat[SIZE][SIZE];

void initmat(int mat[][SIZE]);
void printnonzeroes(int mat[SIZE][SIZE]);
int isprime(int n);

void *tfunc(void *args)
{
	/*
		SIZE/NTHREADS -> 4000 / 4 = 1000
		Thread 0 -> 0 to 999
		Thread 1 -> 1000 to 1999
		Thread 2 -> 2000 to 2999
		Thread 3 ->	3000 to 3999
	*/
	int nthread = *((int *)args);
	int inicio = (SIZE/NTHREADS)*nthread;
	int fin=(SIZE/NTHREADS)*(nthread+1);
	int i,j;
	
	//printf("Hilo %d. Itervalo %d - %d\n",nthread,inicio,fin);
	
	// Eliminar de la matriz todos los números que no son primos	
	for(i=inicio;i<fin;i++)
		for(j=0;j<SIZE;j++)
			if(!isprime(mat[i][j]))
			   mat[i][j]=0;
}

int main()
{
	long long start_ts;
	long long stop_ts;
	long long elapsed_time;
	long lElapsedTime;
	struct timeval ts;
	int i;
	pthread_t tid[NTHREADS];
	int args[NTHREADS];

	// Inicializa la matriz con números al azar
	initmat(mat);
	
	gettimeofday(&ts, NULL);
	start_ts = ts.tv_sec; // Tiempo inicial

	//Crear hilos
	for(i=0;i<NTHREADS;i++)
	{
		args[i]=i;
		pthread_create(&tid[i],NULL,tfunc,&args[i]);
	}
	
	//Esperar hilos
	for(i=0;i<NTHREADS;i++)
	{
		args[i]=i;
		pthread_join(tid[i],NULL);
	}
		
	gettimeofday(&ts, NULL);
	stop_ts = ts.tv_sec; // Tiempo final
	elapsed_time = stop_ts - start_ts;


	printnonzeroes(mat);
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time);
}

void initmat(int mat[][SIZE])
{
	int i,j;
	
	srand(getpid());
	
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++)
			mat[i][j]=INICIAL+rand()%(FINAL-INICIAL);
}

void printnonzeroes(int mat[SIZE][SIZE])
{
	int i,j;
	
	for(i=0;i<SIZE;i++)
		for(j=0;j<SIZE;j++)
			if(mat[i][j]!=0)
				printf("%d\n",mat[i][j]);
}

			   
int isprime(int n)
{
	int d=3;
	int prime=0;
	int limit=sqrt(n);
	
	if(n<2)
		prime=0;
	else if(n==2)
		prime=1;
	else if(n%2==0)
		prime=0;
	else
	{
		while(d<=limit && n%d)
			d+=2;
		prime=d>limit;
	}
	return(prime);
}