// BassTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "BassSoundManager.h"

#pragma comment(lib, "bass.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	CBassSoundManager* pBass = new CBassSoundManager();
	pBass->Initialize();
	pBass->FileLoad("5.mp3");
	pBass->Play(true);

	getchar();

	pBass->Delete();
	delete pBass;

	return 0;
}

