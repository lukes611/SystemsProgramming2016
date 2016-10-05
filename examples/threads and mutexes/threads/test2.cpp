#include <stdio.h>
#include <stdlib.h>
#include "LThread.h"



//define a Job
typedef struct{
	unsigned int id;
	unsigned int from, to, number;
} Job;

void * worker(void * dataIn)
{
	//get job
	Job * job = (Job *) dataIn;
	
	//identify yourself
	printf("I am thread #%i, factoring %i from %i to %i\n", job->id, job->number, job->from, job->to);

	//do the job
	int numFactorsFound = 0;
	unsigned int i;
	for(i = job->from; i <= job->to; i++) if(i > 0 && job->number % i == 0) numFactorsFound++;

	//return the result in number
	job->number = numFactorsFound;

	

	return (void*)job;
}

int main()
{
	int NUMTHREADS = 20;
	int i;
	LThread * threads = (LThread*) malloc(sizeof(LThread) * NUMTHREADS);

	unsigned int numberToFactorize = 50000;
	unsigned int range = numberToFactorize / NUMTHREADS;

	for(i = 0; i < NUMTHREADS; i++)
	{
		//create a job
		Job * job = (Job*)malloc(sizeof (Job));
		job->id = i;
		job->from = i * range + 1;
		job->to = job->from + range - 1;
		job->number = numberToFactorize;
		
		//start a thread
		threads[i] = newLThread();
		threads[i].start(&threads[i], worker, job);

	}

	
	for(i = 0; i < NUMTHREADS; i++){
		Job * job = (Job*) threads[i].join(&threads[i]);

		printf("job %i recorded %i factors\n", job->id, job->number);
		free(job);
	}
	free(threads);
	return 0;
}