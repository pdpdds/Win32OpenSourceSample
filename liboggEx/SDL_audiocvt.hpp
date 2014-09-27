#pragma once

#ifndef _WINDOWS
	#include <stdint.h>
#else
	typedef signed   __int8 int8_t;
	typedef unsigned __int8 uint8_t;
	typedef signed   __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef signed   __int32 int32_t;
	typedef unsigned __int32 uint32_t;
#endif

#include <stdlib.h>
/* Audio format flags (defaults to LSB byte order) */
#define AUDIO_U8	0x0008	/* Unsigned 8-bit samples */
#define AUDIO_S8	0x8008	/* Signed 8-bit samples */
#define AUDIO_U16LSB	0x0010	/* Unsigned 16-bit samples */
#define AUDIO_S16LSB	0x8010	/* Signed 16-bit samples */
#define AUDIO_U16MSB	0x1010	/* As above, but big-endian byte order */
#define AUDIO_S16MSB	0x9010	/* As above, but big-endian byte order */
#define AUDIO_U16	AUDIO_U16LSB
#define AUDIO_S16	AUDIO_S16LSB

typedef int8_t		Sint8;
typedef uint8_t		Uint8;
typedef int16_t		Sint16;
typedef uint16_t	Uint16;
typedef int32_t		Sint32;
typedef uint32_t	Uint32;

/* A structure to hold a set of audio conversion filters and buffers */
struct SDL_AudioCVT {
	int needed;			/* Set to 1 if conversion possible */
	Uint16 src_format;		/* Source audio format */
	Uint16 dst_format;		/* Target audio format */
	double rate_incr;		/* Rate conversion increment */
	Uint8 *buf;			/* Buffer to hold entire audio data */
	int    len;			/* Length of original audio buffer */
	int    len_cvt;			/* Length of converted audio buffer */
	int    len_mult;		/* buffer must be len*len_mult big */
	double len_ratio; 	/* Given len, final size is len*len_ratio */
	void (*filters[10])(struct SDL_AudioCVT *cvt, Uint16 format);
	int filter_index;		/* Current audio conversion function */
};
int SDL_BuildAudioCVT(SDL_AudioCVT *cvt,
	Uint16 src_format, Uint8 src_channels, int src_rate,
	Uint16 dst_format, Uint8 dst_channels, int dst_rate);
int SDL_ConvertAudio(SDL_AudioCVT *cvt);
