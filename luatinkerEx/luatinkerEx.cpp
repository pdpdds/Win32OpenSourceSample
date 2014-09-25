// luatinkerEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include "lua_tinker.h"

#pragma comment(lib, "liblua.lib")

int cpp_func(int arg1, int arg2)
{
	return arg1 + arg2;
}

int main()
{
	// Lua 를 초기화 한다.
	lua_State* L = luaL_newstate();

	// Lua 기본 함수들을 로드한다.- print() 사용
	luaopen_base(L);

	// LuaTinker 를 이용해서 함수를 등록한다.
	lua_tinker::def(L, "cpp_func", cpp_func);

	// sample1.lua 파일을 로드/실행한다.
	lua_tinker::dofile(L, "sample1.lua");

	// sample1.lua 의 함수를 호출한다.
	int result = lua_tinker::call<int>(L, "lua_func", 3, 4);

	// lua_func(3,4) 의 결과물 출력
	printf("lua_func(3,4) = %d\n", result);

	// 프로그램 종료
	lua_close(L);

	return 0;
}