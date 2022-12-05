/*
*
*	pink.c
*	T.L. 2020
*/

#include "alsa/asoundlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

int main( int argc, char **argv )
{
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;
	int err = 0;

	snd_pcm_open( &handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);

	snd_pcm_set_params( 	handle,
				SND_PCM_FORMAT_FLOAT_LE, 	/* little endian*/
				SND_PCM_ACCESS_RW_INTERLEAVED,	/* interleaved */
				1,				/* channels */
				44100,				/* sample rate */
				2,				/* alsa resampling */
				250000);			/* desired latency */
	
	/* run for 4 seconds */
	float x = 0.0f;
	float f = 1000.f;
	float T = 1.f / 44100.f;

	for(int i=0; i < (44100*4); i++)
	{
		x = sin(2 * M_PI * f * (float)i * T);
		err = snd_pcm_writei( handle, &x, 1);
		if( err == -EPIPE )
		{
			fprintf(stderr, "Underrun\n");
		}
	}
	snd_pcm_drain( handle );
	snd_pcm_close( handle );
	return 0;
}

