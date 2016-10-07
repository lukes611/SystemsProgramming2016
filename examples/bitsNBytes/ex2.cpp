#include <stdio.h>
#include <stdlib.h>

// returns 1 or 0 for bit at index, [ 0 1 2 3 4 5 6 7 8 ] = 8 bits to a byte
int getBit(unsigned char byte, int index)
{
	return (byte & 0x80 >> index) != 0;
}

int main()
{
	//binary data
	unsigned char x = 5; //101 in binary

	//let's use basic functions to get and set bits, we can use << >> | & ^ and ~ to manipulate the bits to expose their values
	int i;
	for (i = 0; i < 8; i++)
	{
		printf("%i", getBit(x, i)); //print out each bit
	}
	puts(""); //add end of line char

	//should print out 00000101, 5 in binary

	return 0;
}