#ifndef _INCL_H
#define _INCL_H

#define QSIZE 40

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef WIN32

#include <Winsock2.h>
#include <Windows.h>
#include <conio.h>

void usleep(int waitTime);
#define kbhit _kbhit

#else //for linux

#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
int kbhit();

#endif

#define STDIN_FILENO 0
#define NB_ENABLE 1
#define NB_DISABLE 0


void nonblock(int state);
void BarDisplay(int full, int max);
void BarDisplayEx(int full, int max, int* p1, int *p2, int blocked);


#endif