#include "stdafx.h"

#include <string.h>
#include <stdlib.h>


// wav文件头结构
typedef struct s_wave_header {
	char riff[4];
	unsigned long fileLength;
	char wavTag[4];
	char fmt[4];
	unsigned long size;
	unsigned short formatTag;
	unsigned short channel;
	unsigned long sampleRate;
	unsigned long bytePerSec;
	unsigned short blockAlign;
	unsigned short bitPerSample;
	char     data[4];
	unsigned long dataSize;
}wave_header;


int wavread(char *fname, wave_header *wf, unsigned char ** wavedata, int & datalength)
{
	FILE *fp;
	unsigned int subchunk2size;
	unsigned char *speech;
	//binary form read
	fp = fopen(fname, "rb");
	if (!fp)
	{
		fprintf(stderr, "Can not open the wave file: %s.\n", fname);
		return -1;
	}
	fread(wf, sizeof(wave_header), 1, fp);
	fseek(fp, 20 + 16 + 4, SEEK_SET);
	fread((unsigned int*)(&subchunk2size), 4, 1, fp);
	speech = (unsigned char*)malloc(sizeof(unsigned char)*subchunk2size);
	memset(speech, 0, subchunk2size);
		
	fread(speech, 1, subchunk2size, fp);
	fclose(fp);
	*wavedata = speech;
	datalength = subchunk2size;
	return 0;
}
void wavwrite(char *fname, wave_header *wf, unsigned char * wavedata, int datalength)
{
	FILE * fp = fopen(fname, "wb");
	if (fp)
	{
		
		fwrite(wf, sizeof(wave_header), 1, fp);
		fwrite(wavedata, datalength, 1, fp);
		fclose(fp);


	}
}
extern "C" void G729aInitEncoder();
extern "C" void G729aEncoder(short *speech, unsigned char *bitstream, int nSize);
extern "C" void G729aInitDecoder();
extern "C" void G729aDecoder(unsigned char *bitstream, short *synth_short, int bfi, int nSize);

int main()
{
	wave_header wf;
	unsigned char * wavedata = NULL;
	int datalength = 0;
	G729aInitEncoder();
	G729aInitDecoder();

	wavread("male.wav", &wf, &wavedata, datalength);
	int count = datalength / 160;
	unsigned char * buffer = (unsigned char *)malloc(count * 10);
	unsigned char * pout = (unsigned char *)malloc(datalength);
	int i = 0;
	for (i = 0; i < count; i++)
	{
		G729aEncoder((short*)(wavedata + i * 160), buffer + i * 10, 1);
		G729aDecoder(buffer + i * 10, (short*)(pout + i * 160), 0, 1);
	}
	wavwrite("test1.wav", &wf, pout, datalength);

	free(buffer);
	free(wavedata);
	free(pout);
	return 0;
}