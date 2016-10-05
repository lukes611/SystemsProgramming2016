#include <stdio.h>
#include <stdlib.h>
#include "LThread.h"

//job object
/*
	number		: int						- the number to be factored
	[from,to]	: ints						- a range of numbers to check if they are a factor of number
	slot		: int pointer				- a bit of memory to write from workers to boss
	sync		: unsigned char pointer		- a bit of memory to synchronize communication between a worker and the boss
	id			: int						- an id for a job/worker
	m			: mutex						- a lock to lock access to both sync and slot for workers

*/
typedef struct 
{
	unsigned int number, * slot, from, to;
	unsigned char * sync;
	int id;
	LMutex * m;
} FactorJob;

//c constructor for FactorJob
FactorJob newFactorJob(int id, unsigned int number, unsigned int from, unsigned int to, unsigned int * slot, unsigned char * sync, LMutex * mutex)
{
	FactorJob j;
	j.id = id;
	j.number = number;
	j.from = from;
	j.to = to;
	j.slot = slot;
	j.sync = sync;
	j.m = mutex;
	return j;
}


//worker function
void * workerRoutine(void * d)
{
	FactorJob * j = (FactorJob *) d; //get the job to do

	printf("worker #%i started\n", j->id); //clock on

	
	unsigned int i, numFactors = 0;
	for( i = j->from; i <= j->to; i++) //for each number in job range
	{
		if(j->number % i == 0) //if factor found
		{
			//lock access to slot
			j->m->lock(j->m);

			while(*j->sync != 0) ; //wait for boss to be ready
			*j->slot = i; //give boss the new factor
			*j->sync = 1; //notify boss that factor is ready

			//finished, unlock access
			j->m->unlock(j->m);

			numFactors++;
		}
	}

	printf("worker #%i finished\n", j->id); //clock off

	j->m->lock(j->m); //lock access to sync and slot
	while(*j->sync != 0) ; //wait for boss to be ready
	*j->slot = j->id; //give boss my id
	*j->sync = 2; //notify him that this isn't a factor but saying I'm going home...
	j->m->unlock(j->m); //unlock access

	return (void*)numFactors;
}

int main()
{
	//create mutex
	LMutex m = newLMutex();

	//create workers
	LThread workers[2] = {newLThread(), newLThread()};

	//create some shared memory for threads
	unsigned int slot = -1;
	unsigned char sync = 0;
	unsigned int number = 150634862;

	//create some jobs
	FactorJob j1 = newFactorJob(1, number, 1, number / 2, &slot, &sync, &m);
	FactorJob j2 = newFactorJob(2, number, number / 2 + 1, number, &slot, &sync, &m);
	
	
	int numWorkersWorking = 2;

	puts("boss asking workers to begin their shift");

	workers[0].start(&workers[0], workerRoutine, &j1);
	workers[1].start(&workers[1], workerRoutine, &j2);

	
	while(numWorkersWorking > 0) //while both workers are still working
	{
		if(sync == 1) //if factor found : sync == 1
		{
			printf("factor: %i\n", slot); //print factor
			sync = 0; //reset comm channel
		}else if(sync == 2) //otherwise if worker finished : sync == 2
		{
			numWorkersWorking--;
			int workerId = slot;
			sync = 0;
			printf("boss: okay worker%i you can go home...\n", workerId);
			unsigned int numFactorsFound = (unsigned int) workers[workerId-1].join(&workers[workerId - 1]);
			printf("worker%i found %lu factors today\n", workerId, numFactorsFound);
		}
	}
	
	puts("job complete");
	
	return 0;
}