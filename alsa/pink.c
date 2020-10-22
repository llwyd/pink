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


#define SOS_STAGES 	( 2 )
#define CHANNELS 	( 2 )
#define FS 		( 48000 )

const static float b[][3] = 
{
	{ 1.f         ,-1.81835938f,  0.82094419f},
	{ 0.23529412f, -0.0178079f ,  0.f },
};

const static float a[][3] =
{
	{ 1.         ,-1.94363403,  0.9438566},
	{ 1.        , -0.53567505,  0.f},
};

int main( int argc, char **argv )
{
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;
	int err = 0;

	snd_pcm_open( &handle, "default", SND_PCM_STREAM_PLAYBACK, SND_PCM_ASYNC);

	snd_pcm_set_params( 	handle,
				SND_PCM_FORMAT_FLOAT_LE, 	/* little endian*/
				SND_PCM_ACCESS_RW_INTERLEAVED,	/* interleaved */
				CHANNELS,				/* channels */
				FS,				/* sample rate */
				2,				/* alsa resampling */
				250000);			/* desired latency */
	
	/* run for 4 seconds */
	float xl,xr,yl,yr = 0.0f;

	float y[2];

	float sl[2][2] = {0.0f};
	float sr[2][2] = {0.0f};
	FILE *fp = fopen("pink.txt", "w");
	for(int i=0; i < 32768; i++)
	{
		xl = (((float)rand()/(float)RAND_MAX)*2.f)-1.f;
		xr = (((float)rand()/(float)RAND_MAX)*2.f)-1.f;
		
		for(int j=0; j < SOS_STAGES; j++)
		{
			/* left */
			yl 	 = b[j][0]*xl 		 	+ sl[j][0];
			sl[j][0] = b[j][1]*xl	- a[j][1]*yl 	+ sl[j][1];
			sl[j][1] = b[j][2]*xl	- a[j][2]*yl;

			/* right */
			yr 	 = b[j][0]*xr 		 	+ sr[j][0];
			sr[j][0] = b[j][1]*xr	- a[j][1]*yr 	+ sr[j][1];
			sr[j][1] = b[j][2]*xr	- a[j][2]*yr;
			
			xl = yl;
			xr = yr;
		}
		
		fprintf(fp, "%f\n", yl);

		y[0] = yl;
		y[1] = yr;

		err = snd_pcm_writei( handle, y, 2);
		if( err == -EPIPE )
		{
			fprintf(stderr, "Underrun\n");
		}
	}
	fclose(fp);
	
	snd_pcm_drain( handle );
	snd_pcm_close( handle );
	return 0;
}

