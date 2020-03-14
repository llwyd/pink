/*
*
*	Generating pink noise
*	T.L. 2020
*
*/

#include "portaudio.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FS ( 44100 ) /* Hz */

typedef struct
{
	float b[3]; /* Feed forward*/
	float a[3]; /* Feed backward*/
}sos_t;

typedef struct
{
	sos_t * filter[2];
}pa_filterdata;

static pa_filterdata data;

static int Pink_FilterCallback(void);
static int Pink_VossCallback(void);
static int Pink_WhiteCallback( const void *input,
                                      void *output,
                                      unsigned long frameCount,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *userData );


static int Pink_WhiteCallback( const void *input,
                                      void *output,
                                      unsigned long frameCount,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
					void *userData)
{

	float * out = (float *)output;
		
	for(int i=0;i<frameCount;i++)
	{
		*out++=(((float)rand()/(float)RAND_MAX)*2.f)-1.f;
	}

	return 0;
}
int main( int argc, char ** argv)
{
	PaStream * audioStream;
	
	srand((unsigned int)time(NULL));
	Pa_Initialize();
	Pa_OpenDefaultStream(&audioStream,0,1,paFloat32,FS,128,Pink_WhiteCallback,&data);
	Pa_StartStream(audioStream);
		
	printf("Generating pink noise\n");
	/* Generate White noise */
	int fs = 44100;
	int samples = 4096;
	float white[4096]={0.0f};
	for( int i=0;i<samples;i++)
	{
		white[i]=(float)rand()/(float)RAND_MAX;
		white[i]*=2.0;
		white[i]-=1.0;
	}

	/* Perform filteirng*/
	sos_t filter[2];

	float b1[3]={1.f, -0.07568359f, 0.f};
	float a1[3]={1.f, -0.53567505f, 0.f};

	float b0[3]={1.f, -1.81835938f, 0.82094419f};
	float a0[3]={1.f, -1.94363403f, 0.9438566f};

	/* Storage buffers */
	float s0[2] = {0.0f};
	float s1[2] = {0.0f};

	/* Output buffers */	
	float y0[4096]={0.0f};
	float y1[4096]={0.0f};

	/*
	FILE * fp = fopen("pink.txt","w");
	for(int i=0;i<samples;i++)
	{
		
		y0[i]=		(b0[0]*white[i])+s0[0];
		s0[0]=s0[1]+	(b0[1]*white[i])-(a0[1]*y0[i]);
		s0[1]=		(b0[2]*white[i])-(a0[2]*y0[i]);

		y1[i]=		(b1[0]*y0[i])+s1[0];
		s1[0]=s1[1]+	(b1[1]*y0[i])-(a1[1]*y1[i]);
		s1[1]=		(b1[2]*y0[i])-(a1[2]*y1[i]);
		fprintf(fp,"%g\n",y1[i]);
	}
	fclose(fp);
	*/	

	Pa_Sleep(5000);
	Pa_StopStream(&audioStream);
	Pa_CloseStream(&audioStream);
	Pa_Terminate();
	return 0;
}
