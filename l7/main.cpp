
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prefix.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#else
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#endif

void q(char * msg, int exitStatus)
{
	puts(msg);
	exit(exitStatus);
}

#ifndef WIN32

int ex1()
{
	
	int pipes[2], pipes2[2];

	char forkMessage [] = "fork did not work...";
	char pipeMessage [] = "pipe did not work...";
	char childMessage [] = "child exiting...";

	if(pipe(pipes) == -1) q(pipeMessage, -1);
	if(pipe(pipes2) == -1) q(pipeMessage, -2);
	
	int id = fork();

	if(id == -1) q(forkMessage, -1);
	else if(id == 0) //child
	{
		
		while(1) //keep computing
		{
			char buf[99];
			int nr = read(pipes[0], buf, 100);
			buf[nr] = 0x00;
			if(strcmp(buf, "quit")==0) break;
			
			double val = prefixCalc(buf);
			sprintf(buf, "ans = %lf", val);
			write(pipes2[1], buf, strlen(buf));
		}

		q(childMessage, 5);
	}else //parent
	{
		
		while(1)
		{
			char buf[100];
			fgets(buf, 99, stdin);
			buf[strlen(buf)-1] = 0;
			write(pipes[1], buf, strlen(buf));

			if(strcmp(buf, "quit") == 0) break;

			int nr = read(pipes2[0], buf, 99);
			buf[nr] = 0;
			puts(buf);
		}


		puts("parent waiting for child to end");
		int status;
		wait(&status);
		printf("child exited status %d\n", WEXITSTATUS(status));
	}


	return 0;
}

int ex3()
{
	char p1 [] = "query";
	char p2 [] = "answer";

	//create a named pipe:
	if(mkfifo(p1, 0666) == -1)
	{
		puts("first fifo did not work");
		exit(-1);
	}
	//create another for the response
	if(mkfifo(p2, 0666) == -1)
	{
		puts("second fifo did not work");
		exit(-1);
	}

	int id = fork();
	if(id == -1)
	{
		puts("fork did not work");
	}else if(id == 0) //child
	{
		
		int queryFD = open(p1, O_RDONLY);
		int respFD = open(p2, O_WRONLY);

		while(1) //keep computing
		{
			char buf[99];
			int nr = read(queryFD, buf, 100);
			buf[nr] = 0x00;

			if(strcmp(buf, "quit")==0) break;
			
			double val = prefixCalc(buf);
			sprintf(buf, "ans = %lf", val);
			write(respFD, buf, strlen(buf));
		}

		exit(5);
	}else //parent
	{
	
		int queryFD = open(p1, O_WRONLY);
		int respFD = open(p2, O_RDONLY);
		while(1)
		{
			char buf[100];
			fgets(buf, 99, stdin);
			buf[strlen(buf)-1] = 0;
			write(queryFD, buf, strlen(buf));

			if(strcmp(buf, "quit") == 0) break;

			int nr = read(respFD, buf, 99);
			buf[nr] = 0;
			puts(buf);
		}

		wait(NULL);
	}
	

	unlink(p1);
	unlink(p2);
	return 0;
}

int ex6()
{
	int id = 8922;
	if(shmget(id, 1024, IPC_CREAT | 0644) == -1)
	{
		puts("failed to create shared memory");
		exit(1);
	}

	//setup initial state

	{
		int sm = shmget(id, 1024, 0644);
		void * ptr = shmat(sm, NULL, 0);
		int * ptri = (int*)ptr;
		*ptri = 0;
	}

	int fid = fork();

	if(fid == -1)
	{
		puts("fork failed");
		exit(-1);
	}else if(fid == 0) //child
	{
		int sm = shmget(id, 1024, 0644);
		void * ptr = shmat(sm, NULL, 0);

		int * state = (int *)ptr;
		char * string = (char*)(state+1);

		while(1)
		{
			while(*state != 1) ;//wait for state to be 1

			//get query
			char buf[100];
			strcpy(buf, string);

			if(strcmp("quit", buf) == 0) break;

			//compute answer
			double ans = prefixCalc(buf);
			sprintf(buf, "ans = %lf", ans);
			//set answer
			strcpy(string, buf);
			//set state to 2
			*state = 2;
		}

		exit(5);
	
	}else
	{
		int sm = shmget(id, 1024, 0644);
		void * ptr = shmat(sm, NULL, 0);
		int * state = (int *)ptr;
		char * string = (char*)(state+1);

		while(1)
		{
			while(*state != 0) ; //wait until state is 0

			//ask user for prefix calculation
			printf("enter prefix string: ");
			char buf[100];
			fgets(buf, 99, stdin);
			buf[strlen(buf)-1] = 0;
			
			//write into shared memory
			strcpy(string, buf);

			//set state to 1
			*state = 1;

			if(strcmp(buf, "quit") == 0) // if quit
				break;

			//wait until state is 2
			while(*state != 2) ;

			//read answer
			puts(string);

			//reset state
			*state = 0;

		}

		wait(NULL);
		shmdt(ptr);
	}

	
	return 0;
}


#else //windows

int ex2()
{
	int pipes[2], pipes2[2];

	char pipeMessage [] = "pipe did not work...";
	

	if(_pipe(pipes, 2048, _O_BINARY) == -1) q(pipeMessage, -1);
	if(_pipe(pipes2, 2048, _O_BINARY) == -1) q(pipeMessage, -2);
	
	int _size = 100;
	char * cmdArgs = NULL; 
	
	PROCESS_INFORMATION pi;
	
	STARTUPINFO si;

	
	memset(&si, 0, sizeof si);
	si.cb = sizeof si;

	si.hStdInput = (HANDLE)_get_osfhandle(pipes[0]);
	si.hStdOutput = (HANDLE) _get_osfhandle(pipes2[1]);
	si.dwFlags |= STARTF_USESTDHANDLES;

	char * programName = "C:/Users/luke/Documents/Visual\ Studio\ 2012/Projects/LabSystemsWk7/Debug/Child\ Process.exe";
	char Args[300];
	sprintf(Args, "\"%s\"", programName);
	
	DWORD pid;
	HANDLE phandle;

	if(CreateProcess(programName, Args, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
	{
		pid = pi.dwProcessId;
		phandle = pi.hProcess;
	}else
	{
		puts("create_process failed");
		exit(-5);
	}


	while(1)
	{
		char buf[100];
		fgets(buf, 99, stdin);
		buf[strlen(buf)-1] = 0;
		_write(pipes[1], buf, strlen(buf));

		if(strcmp(buf, "quit") == 0) break;

		int nr = _read(pipes2[0], buf, 99);
		buf[nr] = 0;
		puts(buf);
	}


	WaitForSingleObject(phandle, INFINITE);
	return 0;
}


#endif


int main()
{
	ex6();
	return 0;
}