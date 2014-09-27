// pcreEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "pcrecpp.h"
#include <string>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "pcred.lib")
#pragma comment(lib, "pcrecppd.lib")
#else
#pragma comment(lib, "pcre.lib")
#pragma comment(lib, "pcrecpp.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;
	std::string s;
	pcrecpp::RE re("(\\w+):(\\d+)");
	if (re.error().length() > 0) {
		std::cout << "PCRE compilation failed with error:" << re.error() << "\n";
	}
	if (re.PartialMatch("root:1234", &s, &i))
		std::cout << s << " : " << i << "\n";

}