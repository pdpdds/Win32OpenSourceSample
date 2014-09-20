// libsndfileEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include    <stdio.h>
#include    <sndfile.h>

#pragma comment(lib, "libsndfile-1.lib")

#define BUFFER_LEN      1024
#define    MAX_CHANNELS    6

static void process_data(double *data, int count, int channels);

int main(void)
{   
	static double data[BUFFER_LEN];
	SNDFILE      *infile, *outfile;

	SF_INFO      sfinfo;
	int          readcount;

	if (!(infile = sf_open("input.wav", SFM_READ , &sfinfo)))
	{  
		printf("Not able to open input file %s.\n", "input.wav");
		
		sf_perror(NULL);
		return  1;
	};

	if (sfinfo.channels > MAX_CHANNELS)
	{
		printf("Not able to process more than %d channels\n", MAX_CHANNELS);
		return  1;
	};

	if (!(outfile = sf_open("output.wav", SFM_WRITE, &sfinfo)))
	{
		printf("Not able to open output file %s.\n", "output.wav");
		sf_perror(NULL);
		return  1;
	};

	while ((readcount = sf_read_double(infile, data, BUFFER_LEN)))
	{
		process_data(data, readcount, sfinfo.channels);
		sf_write_double(outfile, data, readcount);
	};

	sf_close(infile);
	sf_close(outfile);

	return 0;
} 

static void process_data(double *data, int count, int channels)
{
	double channel_gain[MAX_CHANNELS] = { 0.5, 0.8, 0.1, 0.4, 0.4, 0.9 };
	int k, chan;

	for (chan = 0; chan < channels; chan++)
	for (k = chan; k < count; k += channels)
		data[k] *= channel_gain[chan];

	return;
} 