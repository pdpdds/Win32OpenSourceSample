// SignalTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <sigc++/trackable.h>
#include <sigc++/signal.h>
#include <sigc++/functors/ptr_fun.h>
#include <sigc++/functors/mem_fun.h>
#include <iostream>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "sigc-vc120-d-2_0.lib")
#else
#pragma comment(lib, "sigc-vc120-2_0.lib")
#endif

SIGC_USING_STD(cout)
SIGC_USING_STD(endl)
SIGC_USING_STD(string)

int foo(int i)   { std::cout << "foo(int " << i << ")" << std::endl; return 1; }
int bar(float i) { std::cout << "bar(float " << i << ")" << std::endl; return 1; }

struct A : public sigc::trackable
{
	int  foo(int i)            { std::cout << "A::foo(int " << i << ")" << std::endl; return 1; }
	void bar(std::string& str) { std::cout << "A::foo(string '" << str << "')" << std::endl; str = "foo was here"; }
};


int _tmain(int argc, _TCHAR* argv[])
{
	// signal
	sigc::signal<int, int> sig;

	// emit empty signal
	sig(0);

	// connect some slots before emitting & test auto-disconnection
	{
		A a;
		sig.connect(sigc::ptr_fun1(&foo));
		sig.connect(sigc::mem_fun1(&a, &A::foo));
		sig.connect(sigc::ptr_fun1(&bar));
		sig(1);
		std::cout << sig.size() << std::endl;
	} // a dies => auto-disconnect
	std::cout << sig.size() << std::endl;
	sig(2);

	// test reference
	A a;
	std::string str("guest book");
	sigc::signal<void, std::string&> sigstr;
	sigstr.connect(sigc::mem_fun(&a, &A::bar));
	sigstr(str);
	std::cout << str << std::endl;

	// test make_slot()
	sig.connect(sigc::ptr_fun1(&foo));
	sigc::signal<int, int> sig2;
	sig2.connect(sig.make_slot());
	sig2(3);

	return 0;
}

