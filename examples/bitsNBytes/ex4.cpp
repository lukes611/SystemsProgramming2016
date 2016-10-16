#include <stdio.h>
#include <stdlib.h>

//get bit for unsigned int
int getBit(unsigned int num, int index)
{
	return (num & 0x80000000 >> index) != 0;
}

//print uint in binary
void printNum(unsigned int num)
{
	int i;
	for (i = 0; i < 32; i++)
	{
		printf("%i", getBit(num, i)); //print out each bit
		if ((i+1) % 4 == 0 && i > 0 && i < 31) printf("-");
	}

}

//sets bit in num at index to value (0 or 1)
void setBit(unsigned int * num, unsigned int index, unsigned int value)
{

	unsigned int op = 0x80000000 >> index; //use op which has a 1 at spot index, 0 elsewhere

	if (value) //truthy - set bit to 1
	{
		//this operation can be done by OR-ing with op
		*num |= op;
	}
	else { //set bit to 0
		   //this operation can be done by AND-ing with the inverse of op,
		   //eg. if op is 0000-0010 and we want the bit value of byte where op has a 1 to be 0, we can AND byte by 1111-1101, 
		*num &= (~op);
	}
}


int main()
{
	//shift rotating, similar to assignment 2
	unsigned int n = 0x00000005; //n is 5

	printf("initial value = ");  printNum(n); printf("\n");

	unsigned int i = 1;
	unsigned int previous = n;

	printf("0.\t");  printNum(n); printf("\n");
	for (; i < 32; i++) //32 times
	{
		unsigned int next = previous; //set next to previous
		next >>= 1; //shift next down by 1
		if (getBit(previous, 31)) //if previous had a bit that got shifted off the edge
			setBit(&next, 0, 1); //set that bit at the front of next
		
		printf("%i.\t", i);  printNum(next); printf("\n");

		//set previous to next
		previous = next;
	}


	return 0;
}