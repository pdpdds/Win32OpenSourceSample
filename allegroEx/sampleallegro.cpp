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
	ALLEGRO_DISPLAY *display;
	ALLEGRO_COLOR bright_green;

	al_init();
	al_install_keyboard();

#ifdef USE_OPENGL
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
#else
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_DIRECT3D_INTERNAL);
#endif

	display = al_create_display(640, 128);
	bright_green = al_map_rgba_f(0.5, 1.0, 0.5, 1.0);

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

	ALLEGRO_FONT* font_ttf = al_load_ttf_font("H2MJRE.ttf", 18, 0);


	WCHAR* szStr = L"Allegro 트루타입 폰트 출력 테스트";
	char charBuffer[256];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, charBuffer, 256, NULL, NULL);

	al_draw_text(font_ttf, bright_green, 10, 10, ALLEGRO_ALIGN_LEFT, charBuffer);
	al_flip_display();

	ALLEGRO_EVENT_QUEUE *queue;
	queue = al_create_event_queue();
	al_register_event_source(queue,
		(ALLEGRO_EVENT_SOURCE*)al_get_keyboard_event_source());

	ALLEGRO_EVENT event;
	while (1) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			break;
	}

	return 0;
}
/*
char szTran[8096];
int nOut = 8096;
USES_CONVERSION;
wchar_t *wc = A2W(iter->second.c_str()); // ANSI to UCS-2
WideCharToMultiByte(CP_UTF8, 0, wc, -1, szTran, nOut, 0, 0); // UCS-2 to UTF-8

int StartPos = 0;
int Len = 0;
float height = 10;
char szTemp[8096] = { 0, };
memcpy(szTemp, szTran, 8096);
do
{
	int EndPos = GetNextLinePos(font_ttf, &szTemp[StartPos]);

	szTemp[StartPos + EndPos] = 0;
	al_draw_text(font_ttf, al_map_rgb(255, 255, 255), 10.0f, height, ALLEGRO_ALIGN_LEFT, &szTemp[StartPos]);
	szTemp[StartPos + EndPos] = szTran[StartPos + EndPos];
	StartPos += EndPos;
	height += 20;
} while (StartPos < strlen(szTran));
*/