#include <scheduler.h>

// Max length of priority queues
#define MAXQUEUES 10

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

// Array of priority queues
QUEUE ready[MAXQUEUES];
QUEUE waitinginevent[MAXTHREAD];

// Global variable for current priority
int priority;
// Global counter for threads
int nthreads = 0;

void scheduler(int arguments)
{
	int old, next;
	int changethread = 0;
	int waitingthread = 0;

	int event = arguments & 0xFF00;
	int callingthread = arguments & 0xFF;

	if (event == NEWTHREAD)
	{
		threads[callingthread].status = READY;
		// Highest priority list
		_enqueue(&ready[0], callingthread);
		priority = 0;
		// Increment thread counter by one
		nthreads += 1;
	}

	if (event == BLOCKTHREAD)
	{
		threads[callingthread].status = BLOCKED;
		_enqueue(&waitinginevent[blockevent], callingthread);
		changethread = 1;
	}

	if (event == ENDTHREAD)
	{
		threads[callingthread].status = END;
		changethread = 1;
	}

	if (event == UNBLOCKTHREAD)
	{
		threads[callingthread].status = READY;
		// Always enqueues highest priority list
		_enqueue(&ready[0], callingthread);
	}

	if (event == TIMER)
	{
		threads[currthread].status = READY;
		/* Adding thread to the next ready queue as long as it's not 
		the last level and the amount of threads is not one
		*/
		if (priority < MAXQUEUES - 1 && nthreads > 1)
		{
			_enqueue(&ready[priority + 1], currthread);
		}
		else
		{
			_enqueue(&ready[priority], currthread);
		}
		changethread = 1;
	}

	if (changethread)
	{
		// Loop variable
		int i;

		old = currthread;

		// Checking if there's threads in each queue
		for (i = 0; i < MAXQUEUES; i++)
		{
			if (!_emptyq(&ready[i]))
			{
				// Get thread out and put it in next
				next = _dequeue(&ready[i]);
				// Point current priority to this found thread
				priority = i;
				break;
			}
		}
		threads[next].status = RUNNING;
		_swapthreads(old, next);
	}
}
