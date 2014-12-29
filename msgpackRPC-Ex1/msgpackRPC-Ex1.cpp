// msgpackRPC-Ex1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <msgpack/rpc/asio.h>
#include <boost/thread.hpp>

#pragma comment(lib, "msgpack.lib")

int main(int argc, char **argv)
{
	const static int PORT = 8070;

	//클라이언트 비동기 연결
	boost::asio::io_service client_io;
	msgpack::rpc::asio::client client(client_io);
	client.connect_async(boost::asio::ip::tcp::endpoint(
		boost::asio::ip::address::from_string("127.0.0.1"), PORT));
	boost::thread clinet_thread([&client_io](){ client_io.run(); });

	//RPC 동기 호출 add 함수 호출
	int result1;
	std::cout << "add, 1, 2 = " << client.call_sync(&result1, "add", 1, 2) << std::endl;

	//RPC 비동기 호출 mul 함수 호출
	auto request2 = client.call_async("mul", 1.2f, 5.0f);

	//비동기 호출이므로 아직 결과가 도착하지 않음
	std::cout << *request2 << std::endl;

	//결과가 올 때까지 대기한 후 결과를 얻는다.
	float result2;
	std::cout << "result = " << request2->sync().convert(&result2) << std::endl;

	// 프라퍼티 값 설정 및 가져오기
	{
		auto request3 = client.call_async("set_number", 64);
		std::cout << *request3 << std::endl;
		request3->sync();
	}
	{
		auto request3 = client.call_async("get_number");
		request3->sync();
		std::cout << *request3 << std::endl;
	}

	getchar();

	//boost asio 종료
	client_io.stop();
	clinet_thread.join();

	return 0;
}
