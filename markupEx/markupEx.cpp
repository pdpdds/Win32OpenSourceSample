// markupEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "markup.h"

typedef struct tag_ConnectorInfo
{
	std::wstring szIP;
	unsigned short port;
	int identifier;	
	std::wstring szDesc;	

	tag_ConnectorInfo()
	{
		port = 0;
		identifier = 0;		
	}

}_ConnectorInfo;


void XMLFileSave()
{
	CMarkup xml;

	xml.AddElem(L"SERVERLIST");
	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10000);
	xml.AddChildElem(L"IDENTIFER", 10000);
	xml.AddChildElem(L"DESC", L"GAME DATABASE SERVER");
	xml.OutOfElem();

	xml.AddChildElem(L"SERVER");
	xml.IntoElem();
	xml.AddChildElem(L"IP", L"127.0.0.1");
	xml.AddChildElem(L"PORT", 10001);
	xml.AddChildElem(L"IDENTIFER", 9000);
	xml.AddChildElem(L"DESC", L"AUTH SERVER");
	xml.OutOfElem();

	std::wstring csXML = xml.GetDoc();
	xml.Save(L"ServerConnection.xml");

	wprintf(L"%s", csXML.c_str());
}

void XMLFileRead()
{
	CMarkup xml;
	xml.Load(L"ServerConnection.xml");

	while (xml.FindChildElem(L"SERVER"))
	{
		_ConnectorInfo connectorInfo;
		xml.IntoElem();

		xml.FindChildElem(L"IP");
		connectorInfo.szIP = xml.GetChildData();

		xml.FindChildElem(L"PORT");
		connectorInfo.port = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"IDENTIFER");
		connectorInfo.identifier = _ttoi(xml.GetChildData().c_str());

		xml.FindChildElem(L"DESC");
		connectorInfo.szDesc = xml.GetChildData();

		xml.OutOfElem();
	}

}


int _tmain(int argc, _TCHAR* argv[])
{
	XMLFileSave();
	XMLFileRead();

	return 0;
}

