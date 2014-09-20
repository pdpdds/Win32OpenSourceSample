// msgpackEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>

#pragma comment(lib, "msgpack.lib")

int _tmain(int argc, _TCHAR* argv[])
{

//데이터
	std::vector<std::string> target;
	target.push_back("Hello,");
	target.push_back("World!");

//데이터 직렬화
	msgpack::sbuffer sbuf;  // simple buffer
	msgpack::pack(&sbuf, target);

//직렬화된 데이터를 역직렬화
	msgpack::unpacked msg;    
	msgpack::unpack(&msg, sbuf.data(), sbuf.size());
	msgpack::object obj = msg.get();

	std::cout << obj << std::endl;    // ["Hello," "World!"]

//역직렬화된 데이터를 원래의 정적인 데이터 타입으로 변환
	std::vector<std::string> result;
	obj.convert(&result);

	return 0;
}

