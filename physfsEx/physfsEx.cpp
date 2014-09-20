// PhysFSSample.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include "physfs.h"

#pragma comment(lib, "physfs.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	int len = WideCharToMultiByte(CP_ACP, 0, argv[0], -1, NULL, 0, NULL, NULL);
	std::string szArg(len, 0);
	WideCharToMultiByte(CP_ACP, 0, argv[0], -1, (LPSTR)szArg.c_str(), len, NULL, NULL);

	if (!PHYSFS_init(szArg.c_str()))
	{
		printf("PHYSFS_init() failed!\n  reason: %s.\n", PHYSFS_getLastError());
		return(1);
	} /* if */

	char* myBuf = 0;
	if (PHYSFS_addToSearchPath("test.zip", 1))
	{
		if (PHYSFS_exists("zip.txt"))
		{
			PHYSFS_file* myfile = PHYSFS_openRead("zip.txt");
			int fileLngth = PHYSFS_fileLength(myfile);
			myBuf = new char[fileLngth];
			int length_read = PHYSFS_read(myfile, myBuf, 1, PHYSFS_fileLength(myfile));
			PHYSFS_close(myfile);

			delete myBuf;
		}
		else
		{
			std::cout << "error, no file found inside zip";
		}
	}
	else
	{
		std::cout << "error, no zip found";
	}
	
	if (!PHYSFS_deinit())
		printf("PHYSFS_deinit() failed!\n  reason: %s.\n", PHYSFS_getLastError());

	return 0;
}

