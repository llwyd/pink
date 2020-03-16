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
#define SOS_STAGES ( 2 )

typedef struct
{
	float y;
	float *s[SOS_STAGES];
	float *b[SOS_STAGES];
	float *a[SOS_STAGES];
	FILE *fp;
}pa_callbackdata;

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

		for(int j=0;j<SOS_STAGES;j++)
		{
			y		=(data->b[j][0]*x)			+ data->s[j][0];
			data->s[j][0]	=(data->b[j][1]*x) - (data->a[j][1]*y) 	+ data->s[j][1];
			data->s[j][1]	=(data->b[j][2]*x) - (data->a[j][2]*y);
			x = y;
		}
		fprintf(data->fp,"%g\n",y);
		*out++ = y;
	}
	data->y=y;

	return 0;
}

int main( int argc, char ** argv)
{
	PaStream * audioStream;
	pa_callbackdata data;

	float b1[3]={ 0.23529412, -0.0178079 ,  0.f };
	float a1[3]={ 1.        , -0.53567505,  0.f};

	float b0[3]={ 1.         ,-1.81835938,  0.82094419};
	float a0[3]={ 1.         ,-1.94363403,  0.9438566};

	float s0[2]={0.0f};
	float s1[2]={0.0f};

	data.b[0]=b0;
	data.b[1]=b1;

	data.a[0]=a0;
	data.a[1]=a1;

	data.s[0]=s0;
	data.s[1]=s1;

	data.y=0.0f;

	data.fp = fopen("pink.txt","w");

	srand((unsigned int)time(NULL));
	Pa_Initialize();

	Pa_OpenDefaultStream(&audioStream,0,1,paFloat32,FS,128,Pink_FilterCallback,&data);
	Pa_StartStream(audioStream);
	printf("Generating pink noise\n");

	Pa_Sleep(5000);
	Pa_StopStream(&audioStream);
	Pa_CloseStream(&audioStream);
	Pa_Terminate();
	fclose(data.fp);
	return 0;
}
