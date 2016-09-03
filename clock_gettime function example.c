#include <stdio.h>
#include <stdlib.h>
#include <time.h>


struct timespec  start1(){
	struct timespec returnValue;
	clock_gettime(CLOCK_REALTIME, &returnValue);
	return returnValue;
}
void  start2(struct timespec * t){ clock_gettime(CLOCK_REALTIME, t); }
double stop(struct timespec t)
{
	struct timespec endtime = start1();
	//return difference in seconds + (difference in nano-seconds) / number of nano-seconds in a second
	return ( endtime.tv_sec - t.tv_sec ) + ( endtime.tv_nsec - t.tv_nsec ) / 1000000000.0;
}

int main()
{
	struct timespec a;
	struct timespec * b = malloc(sizeof(struct timespec));

	
	struct timespec start = start1();
	system("sleep 2");
	double t = stop(start);
	
	printf("time = %lf", t);
	
	
	return 0;
}