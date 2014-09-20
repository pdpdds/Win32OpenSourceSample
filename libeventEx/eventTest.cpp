// eventTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>

#include <event.h>

#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "ws2_32.lib")

void say_hello(int fd, short event, void *arg)
{
	printf("Hello\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	struct event ev;
	struct timeval tv;

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	event_init();
	evtimer_set(&ev, say_hello, NULL);
	evtimer_add(&ev, &tv);

	event_dispatch();

	return 0;
}

