// balorImage.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "balorImage.h"
#include <Windows.h>
#include <balor/graphics/all.hpp>
#include <balor/gui/all.hpp>

using namespace balor::graphics;
using namespace balor::gui;


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Frame frame(L"ImageViewer");
	Bitmap bitmap;

	typedef Menu::ItemInfo Item;
	Item fileMenuItems[] = {
		Item(L"開く(&O)\tCtrl+O", Key::Modifier::ctrl | Key::o),
		Item(),
		Item(L"終了(&X)\tAlt+F4", Key::Modifier::alt | Key::f4)
	};
	Item menuBarItems[] = {
		Item(L"ファイル(&F)", fileMenuItems),
		Item(L"ヘルプ(&H)", [&](Menu::Click&) {
			MsgBox::show(L"Image Viewer Ver 1.0", L"バージョン情報");
		})
	};
	MenuBar menuBar(menuBarItems);
	frame.menuBar(&menuBar);

	frame.onShortcutKey() = [&](Frame::ShortcutKey& e) {
		switch (e.shortcut()) {
		case Key::Modifier::ctrl | Key::o: {
											   OpenFileDialog dialog;
											   dialog.filter(L"画像ファイル\n*.bmp;*.gif;*.png;*.jpg;*.jpeg;*.tiff\n\n");
											   if (dialog.show(frame)) {
												   bitmap = Bitmap(dialog.filePath());
												   if (bitmap != nullptr) {
													   // ウインドウサイズが画像サイズ以下ならスクロールできるようにする。
													   frame.scrollMinSize(bitmap.size());
													   frame.invalidate();
												   }
											   }
											   e.handled(true);
		} break;
		case Key::Modifier::alt | Key::f4: {
											   frame.close();
											   e.handled(true);
		} break;
		}
	};
	frame.onPaint() = [&](Frame::Paint& e) {
		e.graphics().clear();
		if (bitmap != nullptr) {
			Graphics bitmapGraphics(bitmap);
			// スクロール分ずらして画像をコピーする。
			e.graphics().copy(frame.scrollPosition(), bitmapGraphics);
		}
	};

	frame.runMessageLoop();
	return 0;
}