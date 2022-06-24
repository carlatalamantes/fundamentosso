#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <math.h>
#include "semaphoresarr.h"

#define TAMBUFFER 10
#define PROCESSCOUNT 4
#define VELPROD 100000	
#define VELCONS 1000000

/*
* - 4 procesos que buscan números primos (buscadores)
* - 1 proceso que recibe los numeros, los guarda en una lista encadenada y los enseña en orden ascendente (mostrador)
*/

struct STRBUFF
{
	int ent;			
	int sal;				
	int buffer[TAMBUFFER]; 
};

struct STRBUFF *bf;

SEM_ID semarr;

enum
{
	E_MAX,
	N_BLOK,
	S_EXMUT
}; 

struct PRIMELIST
{
	int number;
	struct PRIMELIST *next;
};

struct PRIMELIST *l;

int isPrime(int n);
void buscador(int start, int end, int numbers[]);
void mostrador(int end);
void printList(struct PRIMELIST *list);
void addNumberList(int n);

int main(int argc, char *argv[])
{
	int start_range = atoi(argv[1]);
	int end_range = atoi(argv[2]);
	int count_range = (end_range - start_range - 1);
	int availableNumbers[count_range];
	int shared_mem_id;

	semarr = createsemarray((key_t)0x4008, 3);
	initsem(semarr, E_MAX, TAMBUFFER);
	initsem(semarr, N_BLOK, 0);
	initsem(semarr, S_EXMUT, 1);

	shared_mem_id = shmget((key_t)0x6812, sizeof(struct STRBUFF), 0666 | IPC_CREAT);
	bf = shmat(shared_mem_id, NULL, 0);

	/*
	 * Loop to fill array with numbers between the range
	 */
	for (int i = 0; i < count_range; i++)
	{
		for (int j = start_range; j < end_range; j++)
		{
			availableNumbers[i] = j + 1;
			i++;
		}
	}

	/*
	 * Loop to create 4 processes
	 */
	int chunk_size = round((float)count_range / PROCESSCOUNT);
	int start_limit = 0;
	int end_limit = 0;
	for (int i = 0; i < PROCESSCOUNT; i++)
	{
		if (fork() == 0)
		{
			start_limit = i * chunk_size;
			end_limit = start_limit + chunk_size - 1;
			if(end_limit>count_range){
				end_limit=count_range-1;
			}
			for (int j = start_limit; j <= end_limit; j++)
			{
				/* code */
			}
			
			buscador(start_limit, end_limit, availableNumbers);
		}
	}

	if (fork() == 0)
	{
		mostrador(end_range);
	}

	for (int i = 0; i < PROCESSCOUNT + 1; i++)
	{
		wait(NULL);
	}

	erasesem(semarr);

	shmdt(bf);
	shmctl(shared_mem_id, IPC_RMID, NULL);
	exit(0);
}

int isPrime(int n)
{
	int isPrime = 1;
	int i;

	for (i = 2; i <= n / 2; i++)
	{
		if (n % i == 0)
		{
			isPrime = 0;
			break;
		}
	}
	return isPrime;

}

void buscador(int start, int end, int numbers[])
{
	int i;
	for (i = start; i <= end; i++)
	{
		printf("I %d \n",i);
		printf("END %d \n",end);
		if (isPrime(numbers[i]) || numbers[i]==numbers[end])
		{
			printf("NUMBER: %d\n",numbers[i]);
			semwait(semarr, E_MAX);
			semwait(semarr, S_EXMUT);

			if (numbers[i]!=numbers[end])
			{
				printf("Aquiiiiii \n");
				bf->buffer[bf->ent] = numbers[i];
				printf("Buscador found %d\n",numbers[i]);
			}
			else
			{
				bf->buffer[bf->ent] = 0;
				printf("entre en el else\n");
			}

			bf->ent++;
			if(bf->ent==TAMBUFFER){
				bf->ent=0;
			}	
				
			
			
			usleep(rand()%VELPROD);
		
        	semsignal(semarr,S_EXMUT);	// Libera la sección crítica del buffer
        	semsignal(semarr,N_BLOK);	// Si el consumidor está bloqueado porque el buffer está vacío, lo desbloqueas

        	usleep(rand()%VELPROD);
		
			
		}
	i+=1;
	}
	exit(0);
}

void mostrador(int end)
{
	printf("Mostradorrr\n");
	/* int p_count = PROCESSCOUNT;
	
	int n = 1;
	while (p_count!=0)
	{
		semwait(semarr, N_BLOK);  // Si el buffer está vacío, se bloquea
		semwait(semarr, S_EXMUT); // Asegura el buffer como sección crítica

		n = bf->buffer[bf->sal];

		if (n!=0)
		{
			addNumberList(n);
		}else{
			p_count-=1;
		}

		bf->sal++;
		if (bf->sal == TAMBUFFER)
			bf->sal = 0;

		semsignal(semarr, S_EXMUT);
		semsignal(semarr, E_MAX);
	}

	printList(l);  
*/
	exit(0);
}

void printList(struct PRIMELIST *list)
{
	while (list != NULL)
	{
		printf("%d\n", list->number);
		list = list->next;
	} 
}

void addNumberList(int n)
{
	struct PRIMELIST *current, *next;
	current = (struct PRIMELIST *)malloc(sizeof(struct PRIMELIST));
	int aux;

	if (l == NULL)
	{
		current->number = n;
		current->next = NULL;
		l = current;
	}
	else
	{
		current = l;
		while (current != NULL)
		{
		}
	}
}
