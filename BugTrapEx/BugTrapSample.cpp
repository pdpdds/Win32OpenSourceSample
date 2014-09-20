// BugTrapSample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include "BugTrap.h"


#pragma comment(lib, "BugTrap.lib")

static void SetupExceptionHandler()
{
	BT_InstallSehFilter();
	BT_SetAppName(_T("BugTrapSample"));
	BT_SetSupportEMail(_T("juhang3@daum.net"));
	BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL | BTF_SCREENCAPTURE);
	//BT_SetSupportServer(_T("localhost"), 9999);
	//BT_SetSupportURL(_T("http://www.your-web-site.com"));
}

static void SetupExceptionHandlerNoDialog()
{
	BT_InstallSehFilter();
	BT_SetReportFilePath("C:\\");
	BT_SetAppName(_T("BugTrapSample"));
	BT_SetActivityType(BTA_SAVEREPORT);
	BT_SetReportFormat(BTRF_TEXT);
}

void GenerateBug()
{
	int* pNull = 0;
	*pNull = 5;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SetupExceptionHandlerNoDialog();

	GenerateBug();

	return 0;
}

