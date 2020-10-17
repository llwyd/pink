/*
*
*	pink.c
*	T.L. 2020
*/

#include "alsa/asoundlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>

int main( int argc, char **argv )
{
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;

	snd_pcm_open( &handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);

	snd_pcm_set_params( 	handle,
				SND_PCM_FORMAT_FLOAT_BE, 	/* big endian*/
				SND_PCM_ACCESS_RW_INTERLEAVED,	/* interleaved */
				1,				/* channels */
				44100,				/* sample rate */
				1,				/* alsa resampling */
				250000);			/* desired latency */
	
	/* run for 4 seconds */
	for(int i=0; i < (44100*4); i++)
	{
		float x = (((float)rand()/(float)RAND_MAX)*2.f)-1.f;
		snd_pcm_writei( handle, &x, 1);
	}
	
	snd_pcm_drain( handle );
	snd_pcm_close( handle );
	return 0;
}

