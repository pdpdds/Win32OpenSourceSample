// liboggEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <oggplayer.h>
#include <SDL_audio.h>
#include <SDL.h>
#include <iostream>

#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libogg.lib")
#pragma comment(lib, "libtheora_static.lib")
#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "sdlmain.lib")

void mixaudio(void *data, Uint8 *stream, int len) {
	OggPlayer* ogg = (OggPlayer*)data;
	ogg->audio_read((char*)stream, len);
}

bool init_audio(void* user_data) {
	SDL_AudioSpec fmt;
	fmt.freq = 44100;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;
	fmt.callback = mixaudio;
	fmt.userdata = user_data;

	if (SDL_OpenAudio(&fmt, NULL) < 0) return false;
	SDL_PauseAudio(0);

	return true;
}

SDL_Surface* init_video(int w, int h){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) return NULL;
	return SDL_SetVideoMode(w, h, 0, SDL_SWSURFACE);
}

void on_exit(){
	std::string error(SDL_GetError());
	if (!error.empty())
		std::cerr << "Error: " << error << std::endl;
	SDL_CloseAudio();
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	atexit(on_exit);

	OggPlayer ogg("Example.ogg", AF_S16, 2, 44100, VF_BGRA);
	if (ogg.fail()) {
		SDL_SetError("example.ogg");
		return -1;
	}

	SDL_Surface *screen;
	if (!init_audio((void*)&ogg) || !(screen = init_video(ogg.width(), ogg.height())))
		return -2;

	ogg.play();

	bool running = true;
	while (ogg.playing() && running) {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		if (event.type == SDL_QUIT){ running = false; }

		SDL_LockSurface(screen);
		ogg.video_read((char*)screen->pixels, screen->pitch);
		SDL_UnlockSurface(screen);
		SDL_Flip(screen);
		SDL_Delay(0);
	}
	return 0;
}