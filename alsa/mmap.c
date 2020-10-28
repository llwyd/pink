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
#define LATENCY		( 1000000 ) /* us */

#define BUFFER_SIZE (int)( (float)FS * ( (float)LATENCY / (1000.f * 1000.f)))

int main( int argc, char **argv )
{
    snd_pcm_t *handle;
	snd_pcm_uframes_t offset, frames, size;
	int err = 0;

    int y[BUFFER_SIZE];

	snd_pcm_open( &handle, "plughw:PCH,0", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);

    snd_pcm_set_params( handle,
                        SND_PCM_FORMAT_FLOAT_LE, 	/* little endian*/
                        SND_PCM_ACCESS_MMAP_INTERLEAVED,	/* interleaved */
                        1,				/* channels */
                        FS,				/* sample rate */
                        1,				/* alsa resampling */
                        LATENCY);			/* desired latency */

    int rc = snd_pcm_avail_update( handle );

    const snd_pcm_channel_area_t *my_areas;
   

    frames = BUFFER_SIZE;
    float f = 1000.f;
	float T = 1.f / 48000.f;

    err = snd_pcm_mmap_begin(handle, &my_areas, &offset, &frames);

    /* calculate sine*/
    float * samples = (float *)my_areas[0].addr;
    for(int i =0; i < frames; i++)
    {
        *samples = sin(2 * M_PI * f * (float)i * T);
        samples++;
    }

    snd_pcm_mmap_commit( handle, offset,frames);

    printf("%d\n", offset);
    printf("%d\n", frames);

    snd_pcm_drain( handle );
	snd_pcm_close( handle );
    

    return 0;
}