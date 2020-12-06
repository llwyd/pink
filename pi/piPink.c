/*
*
*	nonblock.c
*	T.L. 2020
*/

#include "alsa/asoundlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define FS 		( 48000 ) /* Hz */
#define LATENCY		( 10000 ) /* us */

#define BUFFER_SIZE (int)( (float)FS * ( (float)LATENCY / (1000.f * 1000.f)))

const float f = 1000.f;
const float T = 1.f / (float)FS;

void Pink( snd_pcm_t * handle, snd_pcm_uframes_t * frames )
{
    const snd_pcm_channel_area_t *my_areas;
    snd_pcm_uframes_t offset;
    static int jdx = 0U;    
    int steps = 0;

    int err = snd_pcm_mmap_begin(handle, &my_areas, &offset, frames);
    if( err < 0)
    {
        printf("Error: %s\n", snd_strerror(err));
    }
    /* calculate sine*/
    float * samples = ((float *)my_areas[0].addr) + (my_areas[0].first / 32);
    steps = my_areas[0].step / 32;
    samples+= offset * steps;

    for(int i =0; i < *frames; i++)
    {
        *samples = sin(2 * M_PI * f * (float)jdx * T);
        jdx++;
        samples++;
    }

    /* Commit new samples */
    err = snd_pcm_mmap_commit( handle, offset, *frames);
    if( err < 0)
    {
        printf("Error: %s\n", snd_strerror(err));
    }
}

int main( int argc, char **argv )
{
    snd_pcm_t *handle;
	snd_pcm_uframes_t offset, frames, size;
	int err = snd_pcm_open( &handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

    if( err < 0)
    {
        printf("Failed to open\n");
    }

    snd_pcm_set_params( handle,
                        SND_PCM_FORMAT_FLOAT_LE, 	        /* little endian*/
                        SND_PCM_ACCESS_MMAP_INTERLEAVED,	/* interleaved */
                        1,				                    /* channels */
                        FS,				                    /* sample rate */
                        2,				                    /* alsa resampling */
                        LATENCY);			                /* desired latency */
    frames = BUFFER_SIZE;


    int rc = snd_pcm_avail_update( handle );
    frames = rc;
    Pink(handle, &frames);
    
    err = snd_pcm_start( handle );
    if( err < 0 )
    {
        printf("Error: %s\n", snd_strerror(err));
    }
    snd_pcm_prepare(handle);
    
    while(1)
    {        
        rc = snd_pcm_avail_update( handle );
        if( rc > 0)
        {
            frames = rc;            
            Pink( handle, &frames);
        }
        else
        {
            /* Do something else */
        }
        
    }
    
    snd_pcm_drain( handle );
	snd_pcm_close( handle ); 

    return 0;
}