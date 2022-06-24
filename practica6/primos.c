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
#define VELBUSC 100000
#define VELMOST 1000000

/*
* - 4 procesos que buscan números primos (buscadores)
* - 1 proceso que recibe los numeros, los guarda en una lista encadenada y los enseña en orden ascendente (mostrador)

DUDASSSS
 Rango de números primos se divide en 4 procesos?
 Mostrador es implementar funciones de lista enlazada con un struct vdd
*/

struct STRBUFF
{
	int ent;				// Donde va a estar el siguiente elemento que voy a meter al buffer
	int sal;				// Donde está el siguiente elemento que voy a sacar del buffer
	char buffer[TAMBUFFER]; // Buffer circular
};

struct STRBUFF *bf;

SEM_ID semarr;

enum
{
	E_MAX,
	N_BLOK,
	S_EXMUT
}; // Semáforos 0,1 y 2

struct PRIMELIST
{
	int number;
	struct PRIMELIST *next;
};

struct PRIMELIST *l;

int isPrime(int n);
void buscador(int start, int end,int numbers[],int max);
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
	int chunk_size = round((float)count_range/4); 
	printf("CHUNKKK %d\n",chunk_size);
	int start_limit = 0;
	int end_limit = 0;
	for (int i = 0; i < 4; i++)
	{
		if (fork() == 0)
		{
			start_limit = i * chunk_size;
			end_limit = start_limit + chunk_size - 1;
			buscador(start_limit,end_limit,availableNumbers,end_range);
		}
	}

	
	if (fork() == 0)
	{
		mostrador(end_range);
	}
	
	for (int i = 0; i < 5; i++)
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

void buscador(int start, int end, int numbers[],int max)
{
	for (int p = start; p < end+1; p++)
	{
		printf("Received %d \n",numbers[p] );
	}
	
	
	int i;

	for (i = start ; i < end+1; i++)
	{
			printf("%d\n",i);

		if (isPrime(i) || i == end)
		{
			semwait(semarr, E_MAX);
			semwait(semarr, S_EXMUT);

			if (i != max)
			{
				bf->buffer[bf->ent] = i;
				//printf("Buscador found %d\n",i);
			}
			else
			{
				bf->buffer[bf->ent] = 0;
			}

			bf->ent++;
			if (bf->ent == TAMBUFFER)
				bf->ent = 0;

			usleep(rand() % VELBUSC);

			semsignal(semarr, S_EXMUT);
			semsignal(semarr, N_BLOK);

			usleep(rand() % VELBUSC);
		}
	} 
	exit(0);
}

void mostrador(int end)
{
	printf("%d enf\n", end);
	/*
	int n = 1;
	while (n != end - 1)
	{
		semwait(semarr, N_BLOK);  // Si el buffer está vacío, se bloquea
		semwait(semarr, S_EXMUT); // Asegura el buffer como sección crítica

		n = bf->buffer[bf->sal];

		if (n)
		{
			addNumberList(n);
		}

		bf->sal++;
		if (bf->sal == TAMBUFFER)
			bf->sal = 0;

		semsignal(semarr, S_EXMUT);
		semsignal(semarr, E_MAX);
	}

	printList(l); */

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
