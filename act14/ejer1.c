//gcc -o ejer2 ejer2.c semaphoresarr.c -lm		
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <math.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "semaphoresarr.h"

#define TAMBUFFER 10

#define VELPROD 100000	
#define VELCONS 1000000

#define FIN 0

#define NPROCS 4


void productor();
void consumidor();

struct STRBUFF {
	int ent;	
	int sal;	
	char buffer[TAMBUFFER];	
};

struct STRBUFF *bf;

SEM_ID semarr;
enum {E_MAX,N_BLOK,S_EXMUT};  

int main()
{
    int res;
    int n;
 	int p;
	int i;
	int shmid;

	srand(getpid());
	
    semarr=createsemarray((key_t) 0x4008,3);

    initsem(semarr,E_MAX,TAMBUFFER);
    initsem(semarr,N_BLOK,0);
    initsem(semarr,S_EXMUT,1);

    printf("Dominó\n");

	shmid=shmget((key_t) 0x6812,sizeof(struct STRBUFF),0666|IPC_CREAT);
	bf=shmat(shmid,NULL,0);
	
	
	p=fork();
	if(p==0)
		productor();

	p=fork();
	if(p==0)
		consumidor();
	
	
	for(i=0;i<2;i++)
	{
		wait(NULL);
	}


    erasesem(semarr);
	
	shmdt(bf);	
	shmctl(shmid,IPC_RMID,NULL);



    exit(EXIT_SUCCESS);
}


void productor()
{
    int n;
    for(n=1;n<=NPROCS;n++)
    {
	
			semwait(semarr,E_MAX);	
        	semwait(semarr,S_EXMUT);

			if(n!=NPROCS+1)
			{
        		bf->buffer[bf->ent]=n;
				printf("Jugador[P%d] listo para jugar\n",n);
			}
			else
				bf->buffer[bf->ent]=FIN;	
			
			bf->ent++;
			if(bf->ent==TAMBUFFER)	
		
 
        	usleep(rand()%VELPROD);
		
        	semsignal(semarr,S_EXMUT);	
        	semsignal(semarr,N_BLOK);	

        	usleep(rand()%VELPROD);
		
    }
    exit(0);
}


void consumidor()
{
    int n=1;
    while(n!=4)
    {
        semwait(semarr,N_BLOK);	// Si el buffer está vacío, se bloquea
        semwait(semarr,S_EXMUT);	// Asegura el buffer como sección crítica 

		n=bf->buffer[bf->sal];
		if(1)
        	printf("\t\t\t\tJugador[P%d] jugando\n",n);

		bf->sal++;
		if(bf->sal==TAMBUFFER)
			bf->sal=0;
				
        usleep(rand()%VELCONS);

        semsignal(semarr,S_EXMUT);	// Libera la SC el buffer
        semsignal(semarr,E_MAX);	// Si el productor está bloqueado porque el buffer estaba lleno, lo desbloquea
        usleep(rand()%VELCONS);
    }
    exit(0);
}
     
