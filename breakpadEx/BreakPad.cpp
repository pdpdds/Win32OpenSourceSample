#include "StdAfx.h"
#include "BreakPad.h"
#include "tchar.h"
#include <process.h>
//#include "SFExceptionHandlerUtil.h"
#include <exception_handler.h>

using namespace google_breakpad;

#pragma comment(lib, "exception_handler.lib")
#pragma comment(lib, "crash_generation_client.lib")
#pragma comment(lib, "common.lib")

static bool BreakPadHandlerCallBack(const wchar_t* dump_path,
									const wchar_t* minidump_id,
									void* context,
									EXCEPTION_POINTERS* exinfo,
									MDRawAssertionInfo* assertion,
									bool succeeded)
{
	UNREFERENCED_PARAMETER(assertion);
	UNREFERENCED_PARAMETER(exinfo);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(dump_path);

	printf("%s is dumped\n", minidump_id);

	return succeeded;
}

BreakPad::BreakPad(void)
: m_pBreakPadHandler(NULL)
{
}

BreakPad::~BreakPad(void)
{
	if(m_pBreakPadHandler)
		delete m_pBreakPadHandler;
}

bool BreakPad::Install()
{
	TCHAR szCurDirectory[MAX_PATH] = {0,};
	TCHAR* pEnd = NULL;
	GetModuleFileName(NULL, szCurDirectory, MAX_PATH);
	pEnd = _tcsrchr(szCurDirectory, _T('\\'));

	if(NULL == pEnd)
	{
		return FALSE;
	}

	*pEnd = _T('\0');
	pEnd = pEnd + 1;

	m_pBreakPadHandler = new ExceptionHandler(szCurDirectory, NULL, BreakPadHandlerCallBack, NULL, true);

	return true;
}