#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ParseLine( char* srcString, char delimiter, char* tokens[], int maxTokens)
{
	int i ;

	int count = 0 ; // a counter for the number of words

	int length = strlen ( srcString ) ; //get the length of the input string

	if( length == 0 ) return count ; //if empty: there are 0 tokens

	//recorded is 0 if a record of the current word does not exist, 1 otherwise
	int recorded = 0 ;

	for( i = 0 ; i < length ; i ++ ) //for each letter
	{
		if ( srcString[ i ] == delimiter ) //if a delimiter is found:
		{
			srcString [ i ] = 0x00; //put a c-string terminator there
			recorded = 0; //the start of any following new words is not yet recorded
		}else
		{
			if ( ! recorded ) tokens [ count ++ ] = srcString + i; //if not recorded: record the start of the string 
			recorded = 1; //it must be recorded now
		}
	}

	return count; //return the number of words
}


int main()
{

	char string [ ] = "  C... why u    have  no    split    function   ? ? ?  ";
	char * toks [ 500 ] ; //create 500 char pointers
	int n = ParseLine( string , ' ' , toks , 500 ) ;

	for ( int i = 0 ; i < n ; i ++ ) 
		printf ( "%i: %s\n" , i + 1 , toks [ i ] ) ;

	return 0;
}