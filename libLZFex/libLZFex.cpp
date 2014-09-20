// libLZFex.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "lzf.h"
#include <string.h>
#include <assert.h>

#ifdef _DEBUG
#pragma comment(lib, "liblzfd.lib")
#else
#pragma comment(lib, "liblzf.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned char* pDest = 0;
	unsigned char* pSrc = 0;
	FILE* fp = 0;
	errno_t err = fopen_s(&fp, "LICENSE", "rb");
	if (err != 0)
		return 0;

	fseek(fp, 0L, SEEK_END);
	int srcLen = ftell(fp);

	fseek(fp, 0L, SEEK_SET);

	pSrc = new unsigned char[srcLen];
	pDest = new unsigned char[srcLen];
	fread(pSrc, srcLen, 1, fp);
	fclose(fp);

//압축
	int destLen = lzf_compress(pSrc, srcLen, pDest, srcLen);
	memset(pSrc, 0, srcLen);

//압축해제
	unsigned int size = lzf_decompress(pDest, destLen, pSrc, srcLen);

	assert(size == srcLen);

	return 0;
}

