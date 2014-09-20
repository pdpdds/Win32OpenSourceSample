// expatEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include "ExpatImpl.h"

#pragma comment(lib, "libexpat.lib")

class CMyXML : public CExpatImpl <CMyXML>
{
public:

	// Constructor 

	CMyXML()
	{
	}

	// Invoked by CExpatImpl after the parser is created

	void OnPostCreate()
	{
		// Enable all the event routines we want
		EnableStartElementHandler();
		EnableEndElementHandler();
		// Note: EnableElementHandler will do both start and end
		EnableCharacterDataHandler();
	}

	// Start element handler

	void OnStartElement(const XML_Char *pszName, const XML_Char **papszAttrs)
	{
		printf("We got a start element %s\n", pszName);
		return;
	}

	// End element handler

	void OnEndElement(const XML_Char *pszName)
	{
		printf("We got an end element %s\n", pszName);
		return;
	}

	// Character data handler

	void OnCharacterData(const XML_Char *pszData, int nLength)
	{
		// note, pszData is NOT null terminated
		printf("We got %d bytes of data\n", nLength);
		return;
	}
};

bool ParseSomeXML(LPCSTR pszFileName)
{
	// Create the parser 

	CMyXML sParser;
	if (!sParser.Create())
		return false;

	// Open the file

	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, pszFileName, "r");
	if (err != 0)
		return false;

	// Loop while there is data

	bool fSuccess = true;
	while (!feof(fp) && fSuccess)
	{
		LPSTR pszBuffer = (LPSTR)sParser.GetBuffer(256); // REQUEST
		if (pszBuffer == NULL)
			fSuccess = false;
		else
		{
			int nLength = fread(pszBuffer, 1, 256, fp); // READ
			fSuccess = sParser.ParseBuffer(nLength, nLength == 0); // PARSE
		}
	}

	// Close the file

	fclose(fp);
	return fSuccess;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ParseSomeXML("example.xml");
	return 0;
}

