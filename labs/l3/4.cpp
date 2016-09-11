#include "include.h"


//- move this to a #include

//----------------------------------
//-- priority (0 = do nothing)
//----------------------------------
int random(int probability)
{
    int value = rand() % 100;
    if (value > probability) return value;
    return 0;
}

//----------------------------------
//-- keep the value within limits
//----------------------------------
int limit(int value, int min, int max)
{
    if (value < min) value = min;
    if (value > max) value = max;
    return value;
}

//----------------------------------
int qs = 0;
int Q[QSIZE];
int QueueAdd(int a, int b)
{
	if(qs < QSIZE){ Q[qs++]=a; return 0; }
	return 1;
}
int QueueDel(int a, int b)
{
	if(qs <= 0) return 1;
	qs--;
	return 0;
}
int QueueDepth(){return qs;}
void QueuePrint(){
	for(int i = 0; i < qs; i++)
	{
		printf("%i ", Q[i]);
	}
}

int main()
{
	int count = QSIZE/2;
	int probadd = 50;
	int probdel = 50;
	nonblock(NB_ENABLE);
	printf("Producer-Consumer simulation \n");
	printf("   The bar graph shows how full the queue is \n");
	printf("   use keys a,z, to adjust producer rate\n");
	printf("   use keys s,x, to adjust consumer rate\n");
	printf("   use q to exit\n");
	printf("----------------------------------\n");

	while(1)
	{
		int blocked = 0;
		int rp = random(probadd);	//- producer priority 
		int rc = random(probdel); 	//- consumer priority
		if (kbhit())
		{
			char c = fgetc(stdin);
			printf("\b");		//- delete it
			fflush(0);

			if (c == 'a') probadd++;
			if (c == 'z') probadd--;
			if (c == 's') probdel++;
			if (c == 'x') probdel--;
			if (c == 'q') break;
			probadd = limit(probadd, 40, 60);
			probdel = limit(probdel, 40, 60);
		}
		if (rp) blocked |= QueueAdd(rp, QSIZE);
		if (rc) blocked |= QueueDel(rc, QSIZE);
		BarDisplayEx(QueueDepth(), QSIZE, 
				&probadd, &probdel, blocked);
		usleep(20000);
	}
	QueuePrint();
}
