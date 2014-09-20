// tcmallocEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "tcmalloc.h"
#include "tinythread.h"
#include <iostream>

using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "libtcmalloc_minimal-debug.lib")
#else
#pragma comment(lib, "libtcmalloc_minimal.lib")
#endif

#pragma comment(lib, "tinythread.lib")

#define MAX_ALLOC 1000000

void AllocDeallocThread(void * aArg)
{
	for (int i = 0; i < MAX_ALLOC; i++)
	{
		char* szbuf = (char*)tc_new(18);
		tc_delete(szbuf);
	}
}

void AllocDeallocDefault(void * aArg)
{
	for (int i = 0; i < MAX_ALLOC; i++)
	{
		char* szbuf = (char*)new char[18];
		delete szbuf ;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwStartTick = GetTickCount();
	tthread::thread t1(AllocDeallocDefault, 0);
	tthread::thread t2(AllocDeallocDefault, 0);
	tthread::thread t3(AllocDeallocDefault, 0);
	tthread::thread t4(AllocDeallocDefault, 0);
	tthread::thread t5(AllocDeallocDefault, 0);
	tthread::thread t6(AllocDeallocDefault, 0);
	tthread::thread t7(AllocDeallocDefault, 0);
	tthread::thread t8(AllocDeallocDefault, 0);
	 
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	DWORD dwLastTick = GetTickCount();

	cout << "ElapsedTime : " << dwLastTick - dwStartTick << endl;
	getchar();

	return 0;
}

 