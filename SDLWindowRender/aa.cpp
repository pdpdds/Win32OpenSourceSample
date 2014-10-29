#include "stdafx.h"
#include "aa.h"

#include <windows.h>

extern "C"
{
	// ffmpeg  
#include "libavcodec/avcodec.h"  
#include "libavformat/avformat.h"  
#include "libswscale/swscale.h"  
	// SDL  
#include "SDL.h"  
#include "SDL_thread.h"  
};






int _tmain(int argc, char* argv[])
{
	///////////////////////////////////////////////////////////////////////////////  
	// ffmpeg  
	// Register all formats and codecs  
	av_register_all();

	// Open video file  
	AVFormatContext *pFormatCtx = NULL;
	//prepare_app_arguments2(&argc, &argv);//Convert UTF-8
	const char* filename = "1.mp4";//argv[1];  
	if (avformat_open_input(&pFormatCtx, filename, NULL, NULL) != 0) {
		return -1; // Couldn't open file  
	}

	// Retrieve stream information  
	if (avformat_find_stream_info(pFormatCtx, NULL) <0) {
		return -1; // Couldn't find stream information  
	}

	// Dump information about file onto standard error  
	av_dump_format(pFormatCtx, 0, filename, 0);
	// Find the first video stream  
	int videoStream = -1;
	for (int i = 0; i <pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1) {
		return -1; // Didn't find a video stream  
	}

	// Get a pointer to the codec context for the video stream  
	AVCodecContext * pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	// Find the decoder for the video stream  
	AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if (pCodec == NULL) {
		fprintf(stderr, "Unsupported codec!\n");
		return -1; // Codec not found  
	}

	// Open codec  
	AVDictionary* optionsDict = NULL;
	if (avcodec_open2(pCodecCtx, pCodec, &optionsDict) <0) {
		return -1; // Could not open codec  
	}
	//Source color format
	AVPixelFormat src_fix_fmt = pCodecCtx->pix_fmt;	//AV_PIX_FMT_YUV420P
	//Objective color format
	AVPixelFormat dst_fix_fmt = PIX_FMT_BGR24;//PIX_FMT_YUV420P
	// Allocate video frame  
	AVFrame* pFrame = av_frame_alloc();
	AVFrame* pFrameYUV = av_frame_alloc();
	if (pFrameYUV == NULL) {
		return -1;
	}

	struct SwsContext* sws_ctx = sws_getContext(
		pCodecCtx->width,
		pCodecCtx->height,
		pCodecCtx->pix_fmt,
		pCodecCtx->width,
		pCodecCtx->height,
		dst_fix_fmt,//PIX_FMT_BGR24,//PIX_FMT_YUV420P,  
		SWS_BILINEAR,
		NULL,
		NULL,
		NULL);

	int numBytes = avpicture_get_size(
		dst_fix_fmt,//PIX_FMT_YUV420P,  
		pCodecCtx->width,
		pCodecCtx->height);
	uint8_t* buffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

	avpicture_fill((AVPicture *)pFrameYUV, buffer, dst_fix_fmt,//PIX_FMT_YUV420P,  
		pCodecCtx->width, pCodecCtx->height);

	// Read frames and save first five frames to disk  
	SDL_Rect sdlRect;
	sdlRect.x = 0;
	sdlRect.y = 0;
	sdlRect.w = pCodecCtx->width;
	sdlRect.h = pCodecCtx->height;

	//////////////////////////////////////////////////////  
	// SDL  
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
		fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
		exit(1);
	}

	SDL_Window* sdlWindow = SDL_CreateWindow("My Game Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pCodecCtx->width, pCodecCtx->height,
		0);
	if (!sdlWindow) {
		fprintf(stderr, "SDL: could not set video mode - exiting\n");
		exit(1);
	}

	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_TARGETTEXTURE);  //Originally the third parameter is 0
	SDL_Texture* sdlTexture = SDL_CreateTexture(
		sdlRenderer,
		SDL_PIXELFORMAT_BGR24,//SDL_PIXELFORMAT_YV12,  
		SDL_TEXTUREACCESS_STATIC,//SDL_TEXTUREACCESS_STREAMING,  
		pCodecCtx->width,
		pCodecCtx->height);
	if (!sdlTexture)
		return -1;
	SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);

	AVPacket packet;
	SDL_Event event;
	while (av_read_frame(pFormatCtx, &packet) >= 0) {
		// Is this a packet from the video stream?  
		if (packet.stream_index == videoStream) {
			// Decode video frame  
			int frameFinished;
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

			// Did we get a video frame?  
			if (frameFinished) {
				sws_scale(
					sws_ctx,
					(uint8_t const * const *)pFrame->data,
					pFrame->linesize,
					0,
					pCodecCtx->height,
					pFrameYUV->data,
					pFrameYUV->linesize
					);

				SDL_UpdateTexture(sdlTexture, &sdlRect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
				SDL_RenderClear(sdlRenderer);
				SDL_RenderCopy(sdlRenderer, sdlTexture, &sdlRect, &sdlRect);
				SDL_RenderPresent(sdlRenderer);
			}
			SDL_Delay(50);
		}

		// Free the packet that was allocated by av_read_frame  
		av_free_packet(&packet);
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			SDL_Quit();
			exit(0);
			break;
		default:
			break;
		}

	}

	SDL_DestroyTexture(sdlTexture);

	// Free the YUV frame  
	av_free(pFrame);
	av_free(pFrameYUV);

	// Close the codec  
	avcodec_close(pCodecCtx);

	// Close the video file  
	avformat_close_input(&pFormatCtx);

	return 0;
}


