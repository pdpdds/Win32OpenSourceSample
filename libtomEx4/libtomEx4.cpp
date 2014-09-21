// libtomEx4.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <tomcrypt.h>

#pragma comment(lib, "tomcrypt.lib")

bool executeRC4(unsigned char* pBuffer, int nBufferLen)
{
	prng_state prng; //의사 랜덤 넘버 제너레이터 

	int err;
	if ((err = rc4_start(&prng)) != CRYPT_OK) //RC4 초기화 
		return 0;
	//키 "Win32OpenSource"를 더해서 엔트로피를 증가시킴 
	if ((err = rc4_add_entropy((const unsigned char*)"Win32OpenSource", 15, &prng)) != CRYPT_OK)
		return 0;

	//RC4 준비 상태.. PRNG Init 
	if ((err = rc4_ready(&prng)) != CRYPT_OK)
		return 0;

	if (rc4_read(pBuffer, nBufferLen, &prng) != nBufferLen)
		return 0;

	//암호화 결과 pBuffer에 저장이 됨 

	rc4_done(&prng);
}


int _tmain(int argc, _TCHAR* argv[])
{

	unsigned char buf[32] = "Hello World";
	int nBufferLen = 11;
	executeRC4(buf, nBufferLen);
	executeRC4(buf, nBufferLen);

	return 0;
}

