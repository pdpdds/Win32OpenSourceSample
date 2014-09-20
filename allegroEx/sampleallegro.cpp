// sampleallegro.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/addons/font/allegro5/allegro_font.h>
#include <allegro5/addons/ttf/allegro5/allegro_ttf.h>
#include <allegro5/addons/image/allegro5/allegro_image.h>

#include <allegro5/addons/native_dialog/allegro5/allegro_native_dialog.h> 

#pragma comment(lib, "allegro.lib")
#pragma comment(lib, "allegro_font.lib")
#pragma comment(lib, "allegro_image.lib")
#pragma comment(lib, "allegro_dialog.lib")
#pragma comment(lib, "allegro_ttf.lib")

int main()
{
	al_init();
	al_install_keyboard();

#ifdef USE_OPENGL
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
#else
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_DIRECT3D_INTERNAL);
#endif

	ALLEGRO_DISPLAY* display = al_create_display(640, 128);

#ifdef USE_OPENGL
#  ifdef USE_MIPMAP
	al_set_window_title(display, "OpenGL (using mipmaps)");
#  else
	al_set_window_title(display, "OpenGL (no mipmaps)");
#  endif
#else
#  ifdef USE_MIPMAP
	al_set_window_title(display, "D3D (using mipmaps)");
#  else
	al_set_window_title(display, "D3D (no mipmaps)");
#  endif
#endif

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

#ifdef USE_MIPMAP
	al_set_new_bitmap_flags(al_get_new_bitmap_flags() | ALLEGRO_MIPMAP);
#endif

	ALLEGRO_FONT* font_ttf = al_load_ttf_font("arial.ttf", 18, 0);
	return 0;
}

