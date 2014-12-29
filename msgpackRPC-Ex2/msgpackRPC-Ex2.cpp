// msgpackRPC-Ex2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <msgpack/rpc/asio.h>
#include <boost/thread.hpp>

#pragma comment(lib, "msgpack.lib")

class SomeClass
{
	int m_number;

public:
	void setNumber(const int &number){ m_number = number; }
	int getNumber()const{ return m_number; }
};


int main(int argc, char **argv)
{
	const static int PORT = 8070;

	//디스패처에 클라이언트 요청을 처리하기 위한 핸들러 등록(add, mul)
	//람다함수로 구현
	msgpack::rpc::asio::dispatcher dispatcher;
	dispatcher.add_handler("add", [](int a, int b)->int{ return a + b; });
	dispatcher.add_handler("mul", [](float a, float b)->float{ return a*b; });
	SomeClass s;
	//프라퍼티의 등록. add_property를 통해서 SomeClass 인스턴스를 등록하고 내부 메소드 getNumber, setNumber 메소드를 등록한다.
	dispatcher.add_property("number", std::function<SomeClass*()>([&s](){ return &s; })
		, &SomeClass::getNumber
		, &SomeClass::setNumber
		);

	//boost asio 서버 시작
	//유저 세션이 요청해왔을 경우 디스패쳐로 해당 내용을 처리한다.
	boost::asio::io_service server_io;
	msgpack::rpc::asio::server server(server_io, [&dispatcher](
		const msgpack::object &msg,
		std::shared_ptr<msgpack::rpc::asio::session> session)
	{
		dispatcher.dispatch(msg, session);
	});

	//리스너 포트 8070을 개방하고 서비스를 시작한다.
	server.listen(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT));
	boost::thread server_thread([&server_io](){ server_io.run(); });

	getchar();

	server_io.stop();
	server_thread.join();

	return 0;
}


