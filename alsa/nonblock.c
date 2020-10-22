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

int main( int argc, char **argv )
{
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;
	int err = 0;

	snd_pcm_open( &handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);

	snd_pcm_set_params( 	handle,
				SND_PCM_FORMAT_FLOAT_LE, 	/* little endian*/
				SND_PCM_ACCESS_RW_INTERLEAVED,	/* interleaved */
				1,				/* channels */
				48000,				/* sample rate */
				2,				/* alsa resampling */
				10000);			/* desired latency */
	
	/* run for 4 seconds */
	float x = 0.0f;
	float f = 1000.f;
	float T = 1.f / 48000.f;

	int len = 48000 * 4;

	float y[len];

	//float buffer[4800];

	//int readIdx 	= 0;  	/* to sound card */
	//int writeIdx 	= 0; 	/* to audio buffer */

	for(int i=0; i < 480; i++)
	{
		x = sin(2 * M_PI * f * (float)i * T);
		y[i] = x;
	}

	int rc = snd_pcm_avail_update( handle );
	printf("%d frames\n", rc);
	int k = 0; /* read index */
	int r = 480; /* write index */
	int samples = 0;
	while( k < len )
	{
		int rc = snd_pcm_avail_update( handle );
		//printf("%d frames\n", rc);
		if( rc < 0 )
		{
			snd_pcm_recover( handle, rc, 0 );
		}
		else if ( rc > 0)
		{
			err = snd_pcm_writei( handle, &y[k], rc);
			if( err == -EPIPE )
			{
				fprintf(stderr, "Underrun\n");
			}
			printf("Values written: %d\n", err);
			k+=err;
		}	
		else
		{
			/* calculate new values */
			if(r<len){
				x = sin(2 * M_PI * f * (float)r * T);
				y[r++] = x;
			}
		}
			
	}	
	snd_pcm_drain( handle );
	snd_pcm_close( handle );
	return 0;
}

