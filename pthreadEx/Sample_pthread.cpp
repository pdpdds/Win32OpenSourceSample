// Sample_pthread.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include "pthread.h"

#pragma comment(lib, "pthreadVC2.lib")

void* doFoo(void* data){

	int t_id = *((int*)data);

	for (int i = 0; i<10; i++){
		printf("thread id(%d) : %d\n", t_id, i);
		Sleep(500);
	}

	return NULL;
}

int _tmain(int argc, _TCHAR* argv[])
{

	pthread_t thread[3];

	int joinStatus;
	int f1 = 1;
	int f2 = 2;
	int f3 = 3;

	pthread_create(&thread[0], NULL, doFoo, (void*)&f1); //스레드를 생성하고 작동시킨다
	pthread_create(&thread[1], NULL, doFoo, (void*)&f2);
	pthread_create(&thread[2], NULL, doFoo, (void*)&f3);

	pthread_join(thread[0], (void**)&joinStatus); //스레드가 끝날때까지 기다린다
	pthread_join(thread[1], (void**)&joinStatus);
	pthread_join(thread[2], (void**)&joinStatus);

	printf("Main End!");

	return 0;
}

