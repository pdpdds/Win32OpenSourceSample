// libtomEx3.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <tomcrypt.h>

#pragma comment(lib, "tomcrypt.lib")


int _tmain(int argc, _TCHAR* argv[])
{
	int idx, err;
	hmac_state hmac;
	unsigned char key[16] = "CGSF";
	unsigned char dst[MAXBLOCKSIZE];
	unsigned long dstlen;
	//해쉬 알고리즘으로 SHA-1 등록 
	if (register_hash(&sha1_desc) == -1)
	{
		printf("Error registering SHA1\n");
		return -1;

		//Descriptor 테이블에서 SHA-1 해쉬 알고리즘의 인덱스 얻음 
		idx = find_hash("sha1");
		dstlen = sizeof(dst);
		//SHA-1 알고리즘과 키 "CGSF"를 이용하여 "hello"라는 데이터의 해싱 결과값을 dst에 얻음 
		err = hmac_memory(idx, key, 4, (const unsigned char*)"hello", 5, dst, &dstlen);
	}

	return 0;
}

