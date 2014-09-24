// balorButton.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "balorButton.h"
#include <Windows.h>
#include <balor/gui/all.hpp>

#ifdef _DEBUG
#pragma comment(lib, "balor_debug.lib")
#else
#pragma comment(lib, "balor.lib")
#endif

using namespace balor::gui;


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Frame frame(L"ButtonAndEvent");

	Button button0(frame, 10, 10, 100, 50, L"button0");
	button0.onClick() = [&](Button::Click&) {
		MsgBox::show(L"button0 clicked");
	};
	Button button1(frame, 10, 80, 100, 50, L"button1", [&](Button::Click&) {
		MsgBox::show(L"button1 clicked");
	});

	frame.runMessageLoop();
	return 0;
}