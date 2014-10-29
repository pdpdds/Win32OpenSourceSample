// SDLWindowRender.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SDLWindowRender.h"
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

#define MAX_LOADSTRING 100

#pragma comment(lib, "sdl2.lib")

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND g_hWnd;
// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SDLWINDOWRENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SDLWINDOWRENDER));


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
	
	SDL_Window* sdlWnd = SDL_CreateWindowFrom(g_hWnd);
	SDL_SetWindowTitle(sdlWnd, "SDL Window - Set by SDL");
	SDL_Surface* s = SDL_GetWindowSurface(sdlWnd);
	SDL_FillRect(s, &s->clip_rect, 0xffff00ff);
	SDL_UpdateWindowSurface(sdlWnd);
	
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWnd, -1, SDL_RENDERER_TARGETTEXTURE);  //Originally the third parameter is 0
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


	/*SDL_InitSubSystem(SDL_INIT_VIDEO);
	SDL_Window* sdlWnd = SDL_CreateWindowFrom(g_hWnd);
	SDL_SetWindowTitle(sdlWnd, "SDL Window - Set by SDL");
	SDL_Surface* s = SDL_GetWindowSurface(sdlWnd);
	SDL_FillRect(s, &s->clip_rect, 0xffff00ff);
	SDL_UpdateWindowSurface(sdlWnd);

	int running = 1;
	SDL_Event event;
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);
	while (running)
	{
		while (SDL_PollEvent(&event))
		{

			switch (event.type){

			//case SDL_USEREVENT:
			case SDL_SYSWMEVENT:
			{
								   SDL_Surface* s = SDL_GetWindowSurface(sdlWnd);
								   SDL_FillRect(s, &s->clip_rect, 0x000000ff);
								   SDL_UpdateWindowSurface(sdlWnd);
			}

				break;
			}
			// Doing the rest of my stuff here 
		}
	}*/

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SDLWINDOWRENDER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SDLWINDOWRENDER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

