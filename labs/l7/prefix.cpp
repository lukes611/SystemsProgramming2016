#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static double prefixSolve(char * * tokens, int * index, char * ops, char * numbers)
{
	char c = tokens[*index][0];
	if(strchr(ops, c)) //is op
	{
		(*index)++;
		//get the first op
		double a = prefixSolve(tokens, index, ops, numbers);
		//get the second op
		double b = prefixSolve(tokens, index, ops, numbers);
		//op them together
		switch(c)
		{
		case '+' : return a + b;
		case '-' : return a - b;
		case '*' : return a * b;
		default  : return a / b;
		}
	}else if(strchr(numbers, c)) //is number
	{
		double ret = (double)atoi(tokens[*index]);
		(*index)++;
		return ret;
	}
	return 0.0;
}

double prefixCalc(char * string)
{
	char numbers [] = "0123456789";
	char ops [] = "+-*/";

	//make a copy so string is protected
	char * string_cpy = (char *) malloc(strlen(string) + 10);
	strcpy(string_cpy, string);


	char * tokens[1000];
	int numTokens = 0;

	char * nextToken = strtok(string_cpy, " ");
	if(!nextToken) return 0.0;

	//generate tokens
	while(nextToken)
	{
		tokens[numTokens] = (char *)malloc(strlen(nextToken) + 5);
		strcpy(tokens[numTokens], nextToken);
		numTokens++;
		nextToken = strtok(NULL, " ");
	}

	//solve
	int startIndex = 0, i;
	double answer = prefixSolve(tokens, &startIndex, ops, numbers);

	//free all data
	for(i = 0; i < numTokens; i++)
		free(tokens[i]);
	free(string_cpy);

	return answer;
}


