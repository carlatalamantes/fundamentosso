#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <math.h>
#include "semaphoresarr.h"

#define PROCESSCOUNT 4
#define FIN 0

/*
* - 4 procesos que buscan números primos (buscadores)
* - 1 proceso que recibe los numeros, los guarda en una lista encadenada y los enseña en orden ascendente (mostrador)
*/

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

int p_count = PROCESSCOUNT;
struct PRIMELIST *plist=NULL;


int main(int argc, char *argv[])
{
	int start_range = atoi(argv[1]);
	int end_range = atoi(argv[2]);
	int count_range = (end_range - start_range);
	int availableNumbers[count_range];

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
		if (isPrime(numbers[i]) )
		{
			printf("Buscador %d\n",numbers[i]);
            insertInList(&plist,numbers[i]);	
		}
	}
	exit(0);
}

void mostrador(int end)
{	
	printf("mostrtadoooor");
	orderList(plist);
	printList(plist);
	exit(0);   
   
}

void printList(struct PRIMELIST *list)
{
	struct PRIMELIST *aux=list;
	printf("jeje %d",aux->number);
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
	printf("%d\n",(*start_ref)->number);
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

