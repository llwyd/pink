
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

int main( int argc, char ** argv )
{
    time_t startTime;
    time_t currentTime;

    double runLength = 10;
    double timeDelta = 0;

    time( &startTime );
    time( &currentTime );

    while( timeDelta < runLength )
    {
        timeDelta = difftime( currentTime, startTime );
        time( &currentTime );
    }

}


