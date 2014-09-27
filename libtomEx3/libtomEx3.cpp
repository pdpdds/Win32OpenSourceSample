// libtomEx3.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <tomcrypt.h>

#pragma comment(lib, "tomcrypt.lib")

int md5Test();
int shaTest();


int _tmain(int argc, _TCHAR* argv[])
{
	shaTest();
	//md5Test();

	return 0;
}

int shaTest()
{
	int idx, err;
	hmac_state hmac;
	unsigned char key[20] = "WIN32 OPENSOURCE";
	unsigned char dst[MAXBLOCKSIZE];
	unsigned long dstlen;
	//해쉬 알고리즘으로 SHA-1 등록 
	if (register_hash(&sha1_desc) == -1)
	{
		printf("Error registering SHA1\n");
		return -1;
	}

	//Descriptor 테이블에서 SHA-1 해쉬 알고리즘의 인덱스 얻음 
	idx = find_hash("sha1");
	dstlen = sizeof(dst);
	//SHA-1 알고리즘과 키 "WIN32 OPENSOURCE"를 이용하여 "hello"라는 데이터의 해싱 결과값을 dst에 얻음 
	err = hmac_memory(idx, key, 4, (const unsigned char*)"hello", 5, dst, &dstlen);

	return err;
}

int  md5Test()
{
	hash_state md;
	char *in = "hello world";
	unsigned char out[16];
	/* setup the hash */
	md5_init(&md);
	/* add the message */
	md5_process(&md, (const unsigned char *)in, strlen((const char*)in));
	/* get the hash in out[0..15] */
	md5_done(&md, out);

	return 0;
}
