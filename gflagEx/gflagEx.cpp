// gflagEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define STRIP_FLAG_HELP 1
#include <gflags/gflags.h>
#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

#pragma comment(lib, "gflags.lib")
#pragma comment(lib, "shlwapi.lib")

DEFINE_int32(port, 0, "What port to listen on");
DEFINE_string(languages, "english", "comma-separated list of languages to offer in the 'lang' menu");

static bool ValidatePort(const char* flagname, gflags::int32 value) {
	if (value > 0 && value < 32768)   // value is ok
		return true;
	printf("Invalid port value for --%s: %d\n", flagname, (int)value);
	return false;
}

static bool ValidateLanguage(const char* flagname, const std::string& value) {
	
	if (value.compare("english") == 0 ||
		value.compare("french") == 0 ||
		value.compare("german") == 0)
		return true;

	return false;
}

int main(int argc, char* argv[])
{
	bool bResult = gflags::RegisterFlagValidator(&FLAGS_port, &ValidatePort);
	assert(bResult == true);

	bResult = gflags::RegisterFlagValidator(&FLAGS_languages, &ValidateLanguage);
	assert(bResult == true);

	gflags::ParseCommandLineFlags(&argc, &argv, true);

	cout << FLAGS_languages << endl;
	cout << FLAGS_port << endl;
	return 0;
}

