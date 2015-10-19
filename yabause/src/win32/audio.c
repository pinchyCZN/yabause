#define WINVER 0x500
#define _WIN32_WINNT 0x500
#include <windows.h>
#include <mmsystem.h>

#if defined(_WIN64)
 typedef unsigned __int64 DWORD_PTR;
#else
 typedef unsigned long DWORD_PTR;
#endif

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 2
#define BITS_PER_SAMPLE 16
#define NUM_AUDIO_BUFFERS 10
#define AUDIO_BUF_SIZE (NUM_CHANNELS*(BITS_PER_SAMPLE/8)*SAMPLE_RATE/10)
HWAVEOUT hwave=0;

struct AUDIO_BUF{
	WAVEHDR wavehdr;
	short buf[AUDIO_BUF_SIZE];
	int pos;
}static audio_bufs[NUM_AUDIO_BUFFERS];

static short visualbuf[AUDIO_BUF_SIZE];
static int visual_pos=0;

int init_audio()
{
	WAVEFORMATEX	wfx;
	int sample_rate=SAMPLE_RATE;
	int num_channels=NUM_CHANNELS;
	int bits_per_sample=BITS_PER_SAMPLE;
	memset(&wfx,0,sizeof(wfx));
    wfx.wFormatTag=WAVE_FORMAT_PCM;
    wfx.nChannels=num_channels;
    wfx.nSamplesPerSec=sample_rate;
    wfx.nBlockAlign=num_channels*bits_per_sample/8;
    wfx.nAvgBytesPerSec=wfx.nBlockAlign*sample_rate;
    wfx.wBitsPerSample=bits_per_sample;
    wfx.cbSize=0;

	waveOutOpen(&hwave,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	if(hwave!=0){
		int i;
		for(i=0;i<NUM_AUDIO_BUFFERS;i++){
			memset(&audio_bufs[i],0,sizeof(audio_bufs[i]));
			audio_bufs[i].wavehdr.lpData=(char*)audio_bufs[i].buf;
			audio_bufs[i].wavehdr.dwBufferLength=sizeof(audio_bufs[i].buf);
			waveOutPrepareHeader(hwave,&audio_bufs[i].wavehdr,sizeof(audio_bufs[i].wavehdr));
			if(audio_bufs[i].wavehdr.dwFlags&WHDR_PREPARED)
				waveOutWrite(hwave,&audio_bufs[i].wavehdr,sizeof(audio_bufs[i].wavehdr));
		}
	}
	return hwave!=0;
}

int clamp_sample(int samp)
{
	samp>>=1;
	//samp<<=1;
	/*
	if(samp>0x7FFF)
		samp=0x7FFF;
	else if(samp<-0x8000)
		samp=-0x8000;
	*/
	return samp;
}
int store_audio(unsigned int *left,unsigned int *right,unsigned int num_samples)
{
	unsigned int i;
	struct AUDIO_BUF *audio_buf=0;
	if(hwave!=0){
		int i;
		for(i=0;i<NUM_AUDIO_BUFFERS;i++){
			if(audio_bufs[i].wavehdr.dwFlags&(WHDR_PREPARED|WHDR_DONE)){
				audio_buf=&audio_bufs[i];
				break;
			}
		}
	}
	if(audio_buf){
		int max=sizeof(audio_buf->buf)/sizeof(short)/2;
		if(num_samples<max)
			max=num_samples;
		audio_buf->pos=0;
		for(i=0;i<max;i++){
			short l,r;
			l=clamp_sample(left[i]);
			r=clamp_sample(right[i]);
			audio_buf->buf[audio_buf->pos++]=l;
			audio_buf->buf[audio_buf->pos++]=r;
			visualbuf[visual_pos++]=l;
			visual_pos%=sizeof(visualbuf)/sizeof(short);
		}
		audio_buf->wavehdr.dwBufferLength=audio_buf->pos*sizeof(short);
		waveOutWrite(hwave,&audio_buf->wavehdr,sizeof(audio_buf->wavehdr));
	}
	return 0;
}
int get_audio_buf(short **p,int *len)
{
	if(p!=0)
		*p=visualbuf;
	if(len!=0)
		*len=sizeof(visualbuf)/sizeof(short);
	return 0;
}

int test_audio()
{
	WAVEFORMATEX	wfx;
	HWAVEOUT hwave=0;
	int sample_rate=44100;
	int num_channels=2;
	int bits_per_sample=16;
	memset(&wfx,0,sizeof(wfx));
    wfx.wFormatTag=WAVE_FORMAT_PCM;
    wfx.nChannels=num_channels;
    wfx.nSamplesPerSec=sample_rate;
    wfx.nBlockAlign=num_channels*bits_per_sample/8;
    wfx.nAvgBytesPerSec=wfx.nBlockAlign*sample_rate;
    wfx.wBitsPerSample=bits_per_sample;
    wfx.cbSize=0;

	waveOutOpen(&hwave,WAVE_MAPPER,&wfx,0,0,CALLBACK_NULL);
	if(hwave!=0){
		int i;
		for(i=0;i<NUM_AUDIO_BUFFERS;i++){
			memset(&audio_bufs[i],0,sizeof(audio_bufs[i]));
			audio_bufs[i].wavehdr.lpData=(char*)audio_bufs[i].buf;
			audio_bufs[i].wavehdr.dwBufferLength=sizeof(audio_bufs[i].buf);
			waveOutPrepareHeader(hwave,&audio_bufs[i].wavehdr,sizeof(audio_bufs[i].wavehdr));
			if(audio_bufs[i].wavehdr.dwFlags&WHDR_PREPARED){
				waveOutWrite(hwave,&audio_bufs[i].wavehdr,sizeof(audio_bufs[i].wavehdr));
			}
		}
		{
			DWORD tick,delta;
			tick=GetTickCount();
			while(TRUE){
				delta=GetTickCount()-tick;
				if(delta>9000)
					break;
				for(i=0;i<NUM_AUDIO_BUFFERS;i++){
					if(audio_bufs[i].wavehdr.dwFlags&(WHDR_PREPARED|WHDR_DONE)){
						int j;
						for(j=0;j<sizeof(audio_bufs[i].buf)/sizeof(short);j++){
							audio_bufs[i].buf[j]=rand();
						}
						audio_bufs[i].wavehdr.dwBufferLength=j*sizeof(short);
						waveOutWrite(hwave,&audio_bufs[i].wavehdr,sizeof(audio_bufs[i].wavehdr));
						printf("write out %i\n",i);
					}
				}
				Sleep(50);

			}
		}
	}
	return 0;
}