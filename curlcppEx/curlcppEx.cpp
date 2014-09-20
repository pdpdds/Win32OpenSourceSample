// curlcppEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "curl_easy.h"

#pragma comment(lib, "curlcpp.lib")

#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

using curl::curl_easy;

int main(int argc, const char **argv) {
	curl_easy easy;
	easy.add(curl_pair<CURLoption, string>(CURLOPT_URL, "http://www.google.it"));
	easy.add(curl_pair<CURLoption, long>(CURLOPT_FOLLOWLOCATION, 1L));
	try {
		easy.perform();
	}
	catch (curl_easy_exception error) {
		// If you want to get the entire error stack we can do:
		vector<pair<string, string>> errors = error.what();
		// Otherwise we could print the stack like this:
		error.print_traceback();
		// Note that the printing the stack will erase it
	}
	return 0;
}