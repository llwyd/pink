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
	float y;
	float s0[2];
	float s1[2];
}pa_callbackdata;

	float b1[3]={1.f, -0.07568359f, 0.f};
	float a1[3]={1.f, -0.53567505f, 0.f};

	float b0[3]={1.f, -1.81835938f, 0.82094419f};
	float a0[3]={1.f, -1.94363403f, 0.9438566f};

static pa_callbackdata data;

static int Pink_FilterCallback( const void *input,
                                      void *output,
                                      unsigned long frameCount,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *callbackData );
static int Pink_VossCallback(void);
static int Pink_WhiteCallback( const void *input,
                                      void *output,
                                      unsigned long frameCount,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags,
                                      void *callbackData );


static int Pink_WhiteCallback( 	const void *input,
                            	void *output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
								void *callbackData)
{
	float * out = (float *)output;
		
	for(int i=0;i<frameCount;i++)
	{
		*out++=(((float)rand()/(float)RAND_MAX)*2.f)-1.f;
	}

	return 0;
}

static int Pink_FilterCallback( const void *input,
                                void *output,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
								void *callbackData)
{
	pa_callbackdata * data = (pa_callbackdata*)callbackData;
	float * out = (float *)output;

	float y = data->y;	
	float x;

	for(int i=0;i<frameCount;i++)
	{
		x = (((float)rand()/(float)RAND_MAX)*2.f)-1.f;
		y			=(b0[0]*x)				+ data->s0[0];
		data->s0[0]	=(b0[1]*x) - (a0[1]*y) 	+ data->s0[1];
		data->s0[1]	=(b0[2]*x) - (a0[2]*y);
		x = y;

		y			=(b1[0]*x)				+ data->s1[0];
		data->s1[0]	=(b1[1]*x) - (a1[1]*y) 	+ data->s1[1];
		data->s1[1]	=(b1[2]*x) - (a1[2]*y);
		y=y/5;
		*out++ = y;
	}
	data->y=y;

	return 0;
}

int main( int argc, char ** argv)
{
	PaStream * audioStream;
	
	srand((unsigned int)time(NULL));
	Pa_Initialize();

	data.y=0.0f;
	data.s0[0]=0.0f;
	data.s0[1]=0.0f;
	data.s1[0]=0.0f;
	data.s1[1]=0.0f;

	Pa_OpenDefaultStream(&audioStream,0,1,paFloat32,FS,128,Pink_FilterCallback,&data);
	Pa_StartStream(audioStream);
		
	printf("Generating pink noise\n");

	Pa_Sleep(5000);
	Pa_StopStream(&audioStream);
	Pa_CloseStream(&audioStream);
	Pa_Terminate();
	return 0;
}

	/* Generate White noise */
	
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