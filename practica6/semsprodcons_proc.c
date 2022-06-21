/* Para compilar este ejemplo requieres hacerlo con el archivo semaphoresarray.c e incluir la librería matemática
 * Por ejemplo:
 *
 *		gcc -o semsprodcons_proc semsprodcons_proc.c semaphoresarr.c -lm		
 *
*/
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <math.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "semaphoresarr.h"

#define TAMBUFFER 10

#define VELPROD 100000	// Microsegundos
#define VELCONS 1000000

#define LIMITE 100
#define FIN 0

void productor();
void consumidor();
int isprime(int n);

struct STRBUFF {
	int ent;	// Donde va a estar el siguiente elemento que voy a meter al buffer
	int sal;	// Donde está el siguiente elemento que voy a sacar del buffer
	char buffer[TAMBUFFER];	// Buffer circular
};

struct STRBUFF *bf;

SEM_ID semarr;
enum {E_MAX,N_BLOK,S_EXMUT};  // Semáforos 0,1 y 2

int main()
{
    // Definición de variables
    int res;
    int n;
 	int p;
	int shmid;

	srand(getpid());
	
    // Creación del arreglo de semáforos
    semarr=createsemarray((key_t) 0x4008,3);

    initsem(semarr,E_MAX,TAMBUFFER);
    initsem(semarr,N_BLOK,0);
    initsem(semarr,S_EXMUT,1);

    printf("Semáforos creados\n");

	// Crear la memoria compartida
	shmid=shmget((key_t) 0x6812,sizeof(struct STRBUFF),0666|IPC_CREAT);
	bf=shmat(shmid,NULL,0);
	
    /* Aquí se crean los procesos */
	p=fork();
	if(p==0)
		productor();

	p=fork();
	if(p==0)
		consumidor();


  
    for(n=0;n<2;n++)
  		wait(NULL);


    // Borrar los semáforos
    erasesem(semarr);
	
	shmdt(bf);	// Desconectar la memoria compartida al terminar
	shmctl(shmid,IPC_RMID,NULL);	// Pedir al SO que elimine la memoria compartida



    exit(EXIT_SUCCESS);
}


void productor()
{
    int n;

    printf("Inicia productor\n");
    for(n=1;n<=LIMITE;n++)
    {
		if(isprime(n) || n==LIMITE)
		{
			semwait(semarr,E_MAX);	// Si se llena el buffer se bloquea
        	semwait(semarr,S_EXMUT);	// Asegurar el buffer como sección crítoca

			if(n!=LIMITE)
			{
        		bf->buffer[bf->ent]=n;				// Poner el número primo encontrado en el buffer
				printf("Productor produce %d\n",n);
			}
			else
				bf->buffer[bf->ent]=FIN;	// Si llegué al límite voy a poner 0 en el buffer
										// El 0 significa FIN o terminado
			
			bf->ent++;
			if(bf->ent==TAMBUFFER)	// Si TAMBUFFER es 5, 0 1 2 3 4
				bf->ent=0;
			
			// bf->ent = (bf->ent + 1) % TAMBUFFER;
		
 
        	usleep(rand()%VELPROD);
		
        	semsignal(semarr,S_EXMUT);	// Libera la sección crítica del buffer
        	semsignal(semarr,N_BLOK);	// Si el consumidor está bloqueado porque el buffer está vacío, lo desbloqueas

        	usleep(rand()%VELPROD);
		}
    }
    exit(0);
}


void consumidor()
{
    int n=1;

    printf("Inicia Consumidor\n");
    while(n)
    {
        semwait(semarr,N_BLOK);	// Si el buffer está vacío, se bloquea
        semwait(semarr,S_EXMUT);	// Asegura el buffer como sección crítica 

		n=bf->buffer[bf->sal];
		if(n)
        	printf("\t\t\t\tConsumidor consume %d\n",n);

		bf->sal++;
		if(bf->sal==TAMBUFFER)
			bf->sal=0;
		
		// bf->sal = (bf->sal + 1) % TAMBUFFER;
		
        usleep(rand()%VELCONS);

        semsignal(semarr,S_EXMUT);	// Libera la SC el buffer
        semsignal(semarr,E_MAX);	// Si el productor está bloqueado porque el buffer estaba lleno, lo desbloquea
        usleep(rand()%VELCONS);

    }
    exit(0);
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