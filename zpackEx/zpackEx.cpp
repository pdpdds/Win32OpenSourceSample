// zpackEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "zpack.h"
#include <string>
#include <iostream>
#include <Windows.h>

#ifdef _DEBUG
#pragma comment(lib, "zpackd.lib")
#else
#pragma comment(lib, "zpack.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	int start = GetTickCount();

	unsigned char buf[1024] = { 0, };
	size_t destSize = sizeof(buf);

	unsigned char buf2[1024] = { 0, };
	size_t destSize2 = sizeof(buf2);


	std::string testData = "동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라 만세 무궁화 삼천리 화려강산 대한사람 대한으로 길이 보전하세 남산위에 저 소나무 철갑을 두른듯 바람서리 불변함은 우리 기상일세 무궁화 삼천리 화려강산 대한사람 대한으로 길이 보전하세";

	for (int i = 0; i < 100; ++i)
	{
	
		CompressLZMA((unsigned char const *)testData.c_str(), testData.size(), buf, destSize);
		UnCompressLZMA(buf, destSize, buf2, destSize2);
	}

	std::cout << (GetTickCount() - start) * 0.001f << std::endl;

	return 0;
}

