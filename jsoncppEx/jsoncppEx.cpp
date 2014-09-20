// jsoncppEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "json\json.h"
#include <iostream>

#pragma comment(lib, "jsoncpp.lib")

void output(const Json::Value & value)
{
	// querying the json object is very simple
	std::cout << value["hello"];
	std::cout << value["number"];
	std::cout << value["array"][0] << value["array"][1];
	std::cout << value["object"]["hello"];
}

int _tmain(int argc, _TCHAR* argv[])
{
	Json::Value fromScratch;
	Json::Value array;
	array.append("hello");
	array.append("world");
	fromScratch["hello"] = "world";
	fromScratch["number"] = 2;
	fromScratch["array"] = array;
	fromScratch["object"]["hello"] = "world";

	output(fromScratch);

	// write in a nice readible way
	Json::StyledWriter styledWriter;
	std::cout << styledWriter.write(fromScratch);

	// ---- parse from string ----

	// write in a compact way
	Json::FastWriter fastWriter;
	std::string jsonMessage = fastWriter.write(fromScratch);

	Json::Value parsedFromString;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(jsonMessage, parsedFromString);
	if (parsingSuccessful)
	{
		std::cout << styledWriter.write(parsedFromString) << std::endl;
	}
	return 0;
}

