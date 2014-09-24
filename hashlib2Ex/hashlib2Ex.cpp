// helloworld.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <hashlibpp.h>

#pragma comment(lib, "hashlib2.lib")

int main(int argc, char **argv, char **env)
{
	hashwrapper *h = new md5wrapper();
	h->test();
	std::string md5 = h->getHashFromString("hello world");
	std::cout << md5 << std::endl;
	delete h;
	return 0;
}
