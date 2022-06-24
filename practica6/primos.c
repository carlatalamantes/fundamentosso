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
#define LIMITE 100
#define FIN 0

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


int isPrime(int n);
void buscador(int start, int end, int numbers[]);
void mostrador(int end);
void printList(struct PRIMELIST *list);
void swap(struct PRIMELIST *a, struct PRIMELIST *b);
void orderList(struct PRIMELIST *start);
void insertInList(struct PRIMELIST **start_ref, int number);


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
	for (int i = 0; i < 4; i++)
	{
		if (fork() == 0)
		{
			start_limit = i * chunk_size;
			end_limit = start_limit + chunk_size - 1;
			if(end_limit>count_range){
				end_limit=count_range-1;
			}
			buscador(start_limit, end_limit, availableNumbers);
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

void buscador(int start, int end, int numbers[])
{
	int i;
	for (i = start; i <= end; i++)
	{
		if (isPrime(numbers[i]) || numbers[i]==numbers[end])
		{
			semwait(semarr, E_MAX);
			semwait(semarr, S_EXMUT);

			if (numbers[i]!=numbers[end])
			{
				bf->buffer[bf->ent] = numbers[i];
			}
			else
			{
				bf->buffer[bf->ent] = 0;
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
	}
	exit(0);
}

void mostrador(int end)
{
	int p_count = PROCESSCOUNT;
	struct PRIMELIST *l=NULL;

	
	int n = 1;
	while (p_count)
	{
		semwait(semarr, N_BLOK);  
		semwait(semarr, S_EXMUT); 

		n = bf->buffer[bf->sal];

		if (n)
		{
			insertInList(&l,n);
		}else{
			p_count-=1;
		}

		bf->sal++;
		if (bf->sal == TAMBUFFER)
			bf->sal = 0;

		semsignal(semarr, S_EXMUT);
		semsignal(semarr, E_MAX);
	}
	orderList(l);
	printList(l);  
	exit(0);
}

void printList(struct PRIMELIST *list)
{
	struct PRIMELIST *aux=list;
	while (aux != NULL)
	{
		printf("%d\n", aux->number);
		aux = aux->next;
	} 
}

void insertInList(struct PRIMELIST **start_ref, int number) 
{ 
    struct PRIMELIST *ptr1 = (struct PRIMELIST*)malloc(sizeof(struct PRIMELIST)); 
    ptr1->number = number; 
    ptr1->next = *start_ref; 
    *start_ref = ptr1; 
} 

void orderList(struct PRIMELIST *start) 
{ 
    int swapped, i; 
    struct PRIMELIST *ptr1; 
    struct PRIMELIST *lptr = NULL; 
  
    if (start == NULL) 
        return; 
  
    do
    { 
        swapped = 0; 
        ptr1 = start; 
  
        while (ptr1->next != lptr) 
        { 
            if (ptr1->number > ptr1->next->number) 
            {
                swap(ptr1, ptr1->next); 
                swapped = 1; 
            } 
            ptr1 = ptr1->next; 
        } 
        lptr = ptr1; 
    } 
    while (swapped); 
} 
  
void swap(struct PRIMELIST *a, struct PRIMELIST *b) 
{ 
    int temp = a->number; 
    a->number = b->number; 
    b->number = temp; 
} 

