// pocoEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/NObserver.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"


using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::TCPServer;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactoryImpl;


#ifdef _DEBUG
#pragma comment(lib, "PocoNetd.lib")
#else
#pragma comment(lib, "PocoNet.lib")
#endif

class EchoConnection : public TCPServerConnection {
public:
	EchoConnection(const StreamSocket& s) : TCPServerConnection(s) 
	{ 
		std::cout << "Connection from " + s.peerAddress().toString();
	}

	void run() {
		StreamSocket& ss = socket();
		try {
			char buffer[256];
			int n = ss.receiveBytes(buffer, sizeof(buffer));
			while (n > 0) {
				ss.sendBytes(buffer, n);
				n = ss.receiveBytes(buffer, sizeof(buffer));
			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
		}
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
		
	Poco::Net::ServerSocket svs(25251);
	
	Poco::Net::TCPServerParams* pParams = new Poco::Net::TCPServerParams();
	pParams->setMaxThreads(4);
	pParams->setMaxQueued(4);
	pParams->setThreadIdleTime(100);

	Poco::Net::TCPServer myServer(new Poco::Net::TCPServerConnectionFactoryImpl<EchoConnection>(), svs, pParams);
	myServer.start();

	getchar();
	
	myServer.stop();

	return 0;
}

