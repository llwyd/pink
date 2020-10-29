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

int main( int argc, char **argv )
{
    snd_pcm_t *handle;
	snd_pcm_uframes_t offset, frames, size;
	int err = 0;

    int y[BUFFER_SIZE];

	err = snd_pcm_open( &handle, "plughw:0,0", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);
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

    

    const snd_pcm_channel_area_t *my_areas;
   

    frames = BUFFER_SIZE;
    float f = 1000.f;
	float T = 1.f / 48000.f;

    int steps = 0;

    //int rc = snd_pcm_avail_update( handle );
    //printf("%d\n",rc);
    
    int first = 1;

    int jdx = 0;
    while(jdx < (48000 * 1))
    {        
        // rc = snd_pcm_avail_update( handle );
        // if( rc < 0 )
		// {
        //     printf("Recovering..\n");
		// 	snd_pcm_recover( handle, rc, 0 );
		// }
        int rc = snd_pcm_avail_update( handle );
        //printf("Available: %d\n",rc);
        if( rc > 0)
        {
            frames = rc;
            err = snd_pcm_mmap_begin(handle, &my_areas, &offset, &frames);
            if( err < 0)
            {
                printf("MMAP ERROR\n");
            }
            /* calculate sine*/
            float * samples = ((float *)my_areas[0].addr) + (my_areas[0].first / 32);
            steps = my_areas[0].step / 32;
            samples+= offset * steps;
            for(int i =0; i < frames; i++)
            {
                *samples = sin(2 * M_PI * f * (float)jdx * T);
                jdx++;
                samples++;
            }

            err = snd_pcm_mmap_commit( handle, offset,frames);
            if( err < 0)
            {
                printf("Error committing\n");
            }
            //printf("Frames written: %d\n",err);
            if( frames > 0 )
            {
            //printf("Offset: %d\n", offset);
            //printf("Frames: %d\n", frames);
            }
            if(first > 0 ){
            err = snd_pcm_start( handle );
            printf("Starting...");
            if( err < 0 )
            {
                printf("Start error: %s\n", snd_strerror(err));
            }
            snd_pcm_prepare(handle);
                first =0;
            }
        }
    }

    //snd_pcm_drain( handle );
	snd_pcm_close( handle );
    

    return 0;
}