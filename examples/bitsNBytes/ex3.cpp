#include <stdio.h>
#include <stdlib.h>

int getBit(unsigned char byte, int index)
{
	return (byte & 0x80 >> index) != 0;
}

void printByte(unsigned char byte)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		printf("%i", getBit(byte, i)); //print out each bit
		if (i == 3) printf("-");
	}

}

//sets bit in byte at index to value (0 or 1)
void setBit(unsigned char * byte, unsigned int index, unsigned int value)
{

	unsigned char op = 0x80 >> index; //use op which has a 1 at spot index, 0 elsewhere

	if (value) //truthy - set bit to 1
	{
		//this operation can be done by OR-ing with op
		*byte |= op;
	}
	else { //set bit to 0
		//this operation can be done by AND-ing with the inverse of op,
		//eg. if op is 0000-0010 and we want the bit value of byte where op has a 1 to be 0, we can AND byte by 1111-1101, 
		*byte &= ~op;
	}
}


int main()
{
	//binary data
	unsigned char x = 5; //101 in binary
	printByte(x);
	puts(""); //add end of line char

	//lets create a custom byte : 2a == 0010-1010
	unsigned char twoA = 0x00; //set all bits to 0
	setBit(&twoA, 2, 1);
	setBit(&twoA, 4, 1);
	setBit(&twoA, 6, 1);

	printByte(twoA);
	printf(" = %x\n", twoA);

	return 0;
}