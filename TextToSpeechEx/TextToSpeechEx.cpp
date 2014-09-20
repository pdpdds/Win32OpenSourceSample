// TextToSpeechEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "audiostream.hpp"

using namespace std;
using namespace audiostream;

int _tmain(int argc, _TCHAR* argv[])
{
	audio_ostream aout;
	aout << "Hello World!" << endl;

	getchar();
	return 0;
}

