/*
*
*       pinkEngine.c
*       T.L. 2020
*
*/
#include <inttypes.h>
#include "pink.h"
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

typedef struct
{
    int h;
    int m;
    int s;
} time_setup_t;

int main( int argc, char ** argv )
{
    /* variables relating to input parameters */
    int inputFlags;
    int inputCount = 0;
    time_setup_t timeSetup;
    memset( &timeSetup, 0x00, sizeof(timeSetup));    

    /* runtime time variables */
    time_t startTime;
    time_t currentTime;
    
    double runTime      = 0;
    double timeDelta    = 0;

    printf("--------------------------------\n");
    printf("\tpinkEngine\n");
    printf("\t T.L 2020\n");
    printf("--------------------------------\n");

    /* configure inputs */
    while((inputFlags = getopt( argc, argv, "h:m:s:")) != -1)
    {   
        switch( inputFlags )
        {
            case 'h':
                timeSetup.h = atoi( optarg );
                inputCount++;
                break;
            case 'm':
                timeSetup.m = atoi( optarg );
                inputCount++;
                break;
            case 's':
                timeSetup.s = atoi( optarg );
                inputCount++;
                break;
        }
    }

    runTime = (double)( ( timeSetup.h * 3600 ) + ( timeSetup.m * 60 ) + timeSetup.s );
    
    if( runTime > 0.0f )
    {
        printf( "Generating Pink Noise for %d hour(s), %d minute(s) and %d second(s)\n", timeSetup.h, timeSetup.m, timeSetup.s );
        printf( "(%.0f second(s))\n", runTime );        
        
        Pink_Init();
        time( &startTime );
        time( &currentTime );

        printf("Generating Pink Noise... \n");
        while( timeDelta < runTime )
        {
            Pink_Run();
            /* run last */
            timeDelta = difftime( currentTime, startTime );
            time( &currentTime );
        }
        Pink_Close();
    }
    else
    {
        printf("Missing time information, returning\n");
    }
    printf("FIN\n");
    return 0;
}

