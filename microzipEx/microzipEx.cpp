// microzipEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "microzip\Zipper.hpp"
#include "microzip\unZipper.hpp"
#include <assert.h>

#ifdef _DEBUG
#pragma comment(lib, "microzip_0-1d.lib")
#else
#pragma comment(lib, "microzip_0-1.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	microzip::Unzipper unzipper;
	assert(true == unzipper.OpenZip("foo.zip"));

	unzipper.Unzip();

	return 0;
}

