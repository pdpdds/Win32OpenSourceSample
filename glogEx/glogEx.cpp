// glogEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <windows.h>
#include <glog/logging.h>

#pragma comment(lib, "libglog.lib")

int main(int argc, char* argv[]) 
{
	char szCurrentDirectory[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, szCurrentDirectory);

	google::InitGoogleLogging(argv[0]);

	strcat_s(szCurrentDirectory, "\\");

	google::SetLogDestination(google::GLOG_INFO, szCurrentDirectory);

	
	LOG(INFO) << "Current Directory " << szCurrentDirectory;

	LOG(INFO) << "Log Test1";
	
	//google::LogToStderr();
	
	LOG(INFO) << "Log Test2";

	LOG(ERROR) << "This should work";

	int x = 4;
	int y = 8;
	LOG_IF(ERROR, x > y) << "This should be also OK";

	google::ShutdownGoogleLogging();
	return 0;
}

