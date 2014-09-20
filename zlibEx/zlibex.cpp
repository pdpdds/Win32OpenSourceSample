// Win32OpenSourceSample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <zlib.h>
#include <assert.h>

#ifdef _DEBUG
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif

#define MAX_DATA_SIZE 10000

int main()
{
	BYTE szSrc[MAX_DATA_SIZE] = "Welcome to Win32 OpenSouce World!!";
	BYTE szDest[MAX_DATA_SIZE] = { 0, };
	int srcSize = strlen((char*)szSrc) + 1;
	int destSize = MAX_DATA_SIZE;

	assert(compress(szDest, (uLongf*)&destSize, szSrc, srcSize) >= 0);	

	memset(szSrc, 0, MAX_DATA_SIZE);
	srcSize = MAX_DATA_SIZE;
	
	assert(uncompress(szSrc, (uLongf*)&srcSize, szDest, destSize) >= 0);

	int ncrc = 0;
	crc32(ncrc, (const Bytef *)szSrc, (unsigned int)srcSize);
	
	return 0;
}