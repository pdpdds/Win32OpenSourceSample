// liblfdsEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "liblfds611.h"
#include <boost/thread.hpp>

#pragma comment(lib, "liblfds611.lib")

#define MAX_QUEUE_DATA 10000
lfds611_queue_state* m_pQueue;

class EnqueueDequeTest
{
public:
	void Enqueue(lfds611_queue_state* pQueue)
	{
		for (int i = 1; i < MAX_QUEUE_DATA; i++)
		{
			int* pData = new int;
			*pData = i;
			lfds611_queue_guaranteed_enqueue(pQueue, pData);
		}
		
	}

	void Dequeue(lfds611_queue_state* pQueue)
	{
		int* pData = NULL;

		while (lfds611_queue_dequeue(m_pQueue, (void**)&pData))
		{			
			delete pData;			
		}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	EnqueueDequeTest test;

	lfds611_queue_new(&m_pQueue, MAX_QUEUE_DATA);

	boost::thread thread1 = boost::thread(boost::bind(&EnqueueDequeTest::Enqueue, &test, m_pQueue));
	thread1.join();

	boost::thread thread2 = boost::thread(boost::bind(&EnqueueDequeTest::Dequeue, &test, m_pQueue));
	boost::thread thread3 = boost::thread(boost::bind(&EnqueueDequeTest::Dequeue, &test, m_pQueue));
	boost::thread thread4 = boost::thread(boost::bind(&EnqueueDequeTest::Dequeue, &test, m_pQueue));
	
	thread2.join();
	thread3.join();
	thread4.join();

	lfds611_queue_delete(m_pQueue, NULL, NULL);

	return 0;
}

