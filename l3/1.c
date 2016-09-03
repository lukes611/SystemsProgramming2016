#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void generate(char * name, double (*f)(double), double start, double end, double step)
{
	double x;
	printf("x\t\t|\t%s\n", name);
	printf("---------------------\n");
	for(x = start; x <= end; x += step)
	{
		printf("%lf\t|\t%lf\n", x, f(x));
	}
}

double sq(double x){return x*x;}
double cb(double x){return x*x*x;}
double f1(double x){return 2*sq(x)-3*x;}
double rec(double x){return 1.0 / x;}
double recsq(double x){rec(sq(x));}
int shell()
{
	int numOptions = 11, i, k, j;
	double start, finalv, step;
	char options[200];
	double (*funcs[11])(double) = {NULL, sin, cos, tan, log, sqrt, sq, cb, f1, rec, recsq};
	char * names[15];
	char * x;
	int selection;

	printf("please enter the starting value of x: "); scanf("%lf", &start);
	printf("please enter the final value of x: ");  scanf("%lf", &finalv);
	printf("please enter the step size of x: ");  scanf("%lf", &step);

	strcpy(options, "sin(x) cos(x) tan(x) log(x) sqrt(x) x*x x*x*x 2*x*x-3*x 1/x 1/(x*x) quit");
	
	puts("Please select one of the following options: \n");
	x = strtok(options, " ");
	
	for(i = 1; x; i++, x = strtok(NULL, " "))
	{
			names[i-1] = x;
 			printf("%i. %s\n", i, x);
	}
	selection = -1;
	printf(": ");
	scanf("%i", &selection);
	if(selection <= 0 || selection > 11) return -1;
	if(selection == 11) return 0;

	generate(names[selection], funcs[selection], start, finalv, step);
	
	return 1;
}

int main()
{
	
	while(1)
	{
		int rv = shell();
		if(rv == 0) break;
		if(rv == -1)
		{
			puts("error");
		}
	}
	puts("bye");
	return 0;
}