#include <stdio.h>
#include <stdlib.h>


double * getData()
{
	unsigned int ret = 0xa5; //ret return value
	double * ptr = (double *)ret; //all pointers are unsigned ints anyway...
	return ptr; //returns 0xa5
}

double getData2() //can use memcpy for this too
{
	unsigned int ret = 0xa5;
	double bucket = 0.1;

	unsigned char * retPtr = (unsigned char*)&ret;
	unsigned char * bucketPtr = (unsigned char*)&bucket;

	int i;
	for (i = 0; i < sizeof(unsigned int); i++)
		bucketPtr[i] = retPtr[i];

	return bucket;
}


int main()
{
	int x = 165; //0xa5
	unsigned int x2 = 0xa5; //0xa5
	unsigned int x3 = 0x000000a5; //0xa5
	unsigned int x4 = (unsigned int)getData(); //0xa5
	double x5 = getData2(); //0xa5
	unsigned int x6 = *(unsigned int *)&x5; //0xa5

	//printf with %x prints out hex
	printf("%x\n", x);
	printf("%x\n", x2);
	printf("%x\n", x3);
	printf("%x\n", x4);
	printf("%x\n", x5);
	printf("%x\n", x6);

	//x6 explained - x6 allows us to re-interperet x5 as a uint (we can set it to other uints)
	unsigned int _x6 = //step 5 : assign
		* //step 4 : dereference to get the actual uint value
		(unsigned int *) //step 3 : tell c-compiler to generate code which interprets this pointer as pointing to a uint
		& //step 2 : get pointer to it
		x5; //step 1 : with x5 (which is actually a5)

	return 0;
}