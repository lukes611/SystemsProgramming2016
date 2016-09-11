#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>

/*

TESTS: linux, note windows select only works on sockets (do not use for keyboard, for keyboard reasearch the kbhit function)

select is used to check whether we can read from a socket or file descriptor

steps:
1. create a list of sockets/file-descriptors to check
2. clear the list
3. add each sockets/file-descriptors to check
4. set a maximum time to wait on
5. check
6. if can read -> read else do something else

*/

int main ( )
{
    
    
    fd_set listToWaitOn ; //1. the list is in the form of a struct called fd_set
    
    FD_ZERO ( & listToWaitOn ) ; //2. FD_ZERO clears the list, we must pass it a pointer to the list so we use the & operator
    
    FD_SET ( STDIN_FILENO , & listToWaitOn ) ; //3. we are adding one file-descriptor (reading from keyboard) to the list
    
    //4. let's wait a maximum of 5 second and 7 microseconds
    
    struct timeval maxWait ; 
    
    maxWait . tv_sec = 5 ; //5 second
    
    maxWait . tv_usec = 7 ; //7 microseconds
    
    // end 4.
    
    int largestFileDescriptorPlus1 = STDIN_FILENO + 1; // we need to know the largest file descriptor + 1 to give to select
    
    printf("type something (or don't): \n"); //whether you type something within 5.000007 seconds or not, we can tell using select
    //try out both
    
    int numberOfFileDescriptorsReadyToRead = select ( // 5.
        
        largestFileDescriptorPlus1 , 
        
        & listToWaitOn , 
        
        ( fd_set * ) 0 , //we don't want to wait on any read lists
        
        ( fd_set * ) 0 , //nor exeptions
        
        & maxWait
        
    ) ;
    
    
    printf ( "number of file-descriptors ready to read: %d\n" , numberOfFileDescriptorsReadyToRead ) ;
    
    // 6.
    
    if ( FD_ISSET ( STDIN_FILENO , & listToWaitOn  ) ) //check if STDIN_FILENO is ready to read 
    {
        char buffer [ 100 ] ; //create a buffer
        
        fgets ( buffer , 90 , stdin ) ; // read in one line up to \n
        
        printf ( "you entered : %s\n" , buffer ) ;
    }
    
    else 
    {
        puts ( "you entered nothing" ) ;
    }
    
    return 0 ;
}