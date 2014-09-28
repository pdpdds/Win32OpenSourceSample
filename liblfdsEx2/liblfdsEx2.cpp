// liblfdsTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "liblfds611.h"

#pragma comment(lib, "liblfds611.lib")

bool g_bPushComplete = false;


static void PushThread(void* Args)
{
	struct lfds611_stack_state* pStack = (struct lfds611_stack_state*)Args;

	for(int i = 0; i < 3000; i++)
	{
		int* p = new int;
		*p = i;

		lfds611_stack_guaranteed_push(pStack, p);		
		printf("push success %d\n", GetCurrentThreadId());
	}
}

static void PopThread(void* Args)
{
	struct lfds611_stack_state* pStack = (struct lfds611_stack_state*)Args;

	while (g_bPushComplete == false)
	{
		int* t = NULL;
		int i = lfds611_stack_pop(pStack, (void**)&t);

		if(t != NULL)
		{
			printf("pop success %d\n", GetCurrentThreadId());
			delete t;
		}
		else
		{
			//printf("pop fail\n");
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	struct lfds611_stack_state* pStack = NULL;
	lfds611_stack_new(&pStack, 10000);

	ACE::init();

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	int OptimalThreadCount =si.dwNumberOfProcessors;

	int pushThreadGroup = ACE_Thread_Manager::instance()->spawn_n(OptimalThreadCount, (ACE_THR_FUNC)PushThread, pStack, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY, 1);
	int popThreadGroupID = ACE_Thread_Manager::instance()->spawn_n(OptimalThreadCount, (ACE_THR_FUNC)PopThread, pStack, THR_NEW_LWP, ACE_DEFAULT_THREAD_PRIORITY, 2);
	
	ACE_Thread_Manager::instance()->wait_grp(pushThreadGroup);

	g_bPushComplete = true;

	ACE_Thread_Manager::instance()->wait_grp(popThreadGroupID);

	ACE::fini();

	return 0;
}