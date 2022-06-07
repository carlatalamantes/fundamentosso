#include <scheduler.h>

#define QUEUES 10


extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

//QUEUE ready[QUEUES];
QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

//int currprior;

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	int i;
	int nextlevel;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		threads[callingthread].status=READY;
		//_enqueue(&ready[0],callingthread);
	}
	
	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
	}

	
	if(changethread)
	{
		old=currthread;
		next=_dequeue(&ready);
		
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

}
