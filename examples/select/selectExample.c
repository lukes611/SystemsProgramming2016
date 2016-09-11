#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

int canRead(int fd, int seconds, int microseconds){
    fd_set l;
    FD_ZERO(&l);
    FD_SET(fd, &l);
    struct timeval t = {seconds, microseconds};
    return select(fd + 1, &l, NULL, NULL, &t) >  0; //can also use FD_ISSET but we already know we only have 1 item in list anyway
}


int main(){
    char buffer[100] = "", i;
    printf("type something and hit enter: \n");
    for(i = 0;i < 127 && strlen(buffer) == 0; ++i){
        if(canRead(STDIN_FILENO, 0, 500000)) fgets(buffer, 99, stdin), printf("you typed : %s\n", buffer);
        else printf("\rwaiting %3d%c%c%c     \r",i,i%4==0?' ':'.',(i/2)%2==0?' ':'.',(i+1)%4==0?'.':' '), fflush(stdout);
    }
    return 0;
}


