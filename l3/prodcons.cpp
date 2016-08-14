#include "include.h"




//----------------------------------
//--- allow nonblocking reading ----
//----------------------------------
#ifdef WIN32
void nonblock(int state){}

void usleep(int waitTime) {
    __int64 time1 = 0, time2 = 0, freq = 0;
	QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < waitTime);
}

#else
void nonblock(int state)
{
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);  //- get the terminal state

    if (state == NB_ENABLE)
    {
        ttystate.c_lflag &= ~ICANON;   //- turn off canonical mode
        ttystate.c_cc[VMIN] = 1;       //- minimum input chars read
    }
    else if (state==NB_DISABLE)
    {
       ttystate.c_lflag |= ICANON;     //- turn on canonical mode
    }
    //- set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int kbhit()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); 
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

#endif

void BarDisplayEx(int full, int max, int* p1, int *p2, int blocked)
{
	char s1[100], s2[100];
    int i;
    for (i = 0; i < max; i++) printf("#");
    if (p2) 
    {
		sprintf(s1, "     P:%2d C:%2d  ", *p1, *p2);
		if(blocked) strcpy(s2, "BLOCKED");
		else strcpy(s2, "       ");
		printf("%s%s", s1, s2);
        //printf("     P:%2d C:%2d  ", *p1, *p2);
        //if (blocked) printf("BLOCKED");
        //else 	     printf("       ");
        for (i = 0; i < strlen(s1)+strlen(s2); i++) printf("\b");
		
}
	putc('\r', stdout);
    for (i = 0; i < full; i++) printf("_");
	putc('\r', stdout);
    fflush(0);
}

void BarDisplay(int full, int max)
{
    BarDisplayEx(full, max, 0, 0, 0);
}

