/*
 * En esta versión el receive es bloquente 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <math.h>



struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    unsigned int num;   /* message data */
};

int queue;	// Buzón de mensajes
int process_count = 4;	// Cantidad de procesos que se van a crear

void buscador();
void mostrador();

#define ITERS 10
#define FIN 65535
int isPrime(int n);
int main(int argc, char *argv[])
{
	int start_range = atoi(argv[1]);
	int end_range = atoi(argv[2]);
	int count_range = (end_range - start_range );
	int availableNumbers[count_range];
	int pid;
	int status;


	// Crear buzón de mensajes 
	queue=msgget(0x1234,0666|IPC_CREAT);
	if(queue==-1)
	{
		fprintf(stderr,"No se pudo crear el buzón\n");
		exit(1);
	}
	
	
	
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
	int chunk_size = round((float)count_range / 4);
	int start_limit = 0;
	int end_limit = 0;
	for (int i = 0; i < 4; i++)
	{
		process_count--;

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

	// Crea un proceso donde va a ejecutarse el emisor
	pid=fork();
	if(pid==0)
		mostrador( );	// El hijo ejecuta el mostrador
	
	// Esperar a que los dos procesos terminen
	for (int i = 0; i < 5; i++)
	{
		wait(&status);
	}
	
	msgctl(queue, IPC_RMID, NULL);
}

void buscador(int start,int end, int numbers[])
{

	int i;
	struct msgbuf mensaje;
	mensaje.mtype=1;
	for(i=start;i<=end;i++)
	{

		
		if(isPrime(numbers[i]))
		{
				mensaje.num=numbers[i];
			
				msgsnd(queue,&mensaje,sizeof(mensaje.num),0);
		} else{
			mensaje.num=0;
		}  

	}
	//Enviar el mensaje con la constante FIN
	
		mensaje.num=FIN;
	
	
	msgsnd(queue,&mensaje,sizeof(struct msgbuf),IPC_NOWAIT);
	
	
	exit(0);	// Termina el emisor
}


void mostrador()
{
	int flag=4;
	
	struct msgbuf mensaje;
	do
	{
		msgrcv(queue,&mensaje,sizeof(struct msgbuf),1,0);
		if(mensaje.num == FIN){
			flag--;
		}
		else if(mensaje.num != 0 ){

			printf("%d  \n",mensaje.num);
			fflush(stdout);
		}
		//sleep(1);
		 // Mientras no sea fin 
	} while (flag > 0);


	exit(0);	// Termina el receptor
}

int isPrime(int n)
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
