// HoardEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

#if defined(USE_HOARD)
#pragma comment(lib, "libhoard.lib") 
#endif 

#define ALLOCATE_SIZE 1000
int _tmain(int argc, _TCHAR* argv[])
{
	
	void * m = malloc(ALLOCATE_SIZE);
	void * c = calloc(ALLOCATE_SIZE, ALLOCATE_SIZE);
	int  * n = new int[ALLOCATE_SIZE];

	cout << "Result of malloc = " << m << endl
		<< "Result of calloc = " << c << endl
		<< "Result of new    = " << n << endl;

	return 0;
}

