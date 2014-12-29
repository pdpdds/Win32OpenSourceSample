// torrentDownloader.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"

#pragma comment(lib, "torrent-rasterbar.lib")
#pragma comment(lib, "DbgHelp.lib")


int main(int argc, char* argv[])
{
	using namespace libtorrent;

	//파라메터로 토렌트 파일을 입력한다.
	if (argc != 2)
	{
		fputs("usage: ./simple_client torrent-file\n"
			"to stop the client, press return.\n", stderr);
		return 1;
	}

	session s;
	error_code ec;
	s.listen_on(std::make_pair(6881, 6889), ec);
	if (ec)
	{
		fprintf(stderr, "failed to open listen socket: %s\n", ec.message().c_str());
		return 1;
	}

	//컨텐츠가 저장될 폴더를 지정한다.
	add_torrent_params p;
	p.save_path = "./";
	p.ti = new torrent_info(argv[1], ec);
	if (ec)
	{
		fprintf(stderr, "%s\n", ec.message().c_str());
		return 1;
	}


	//토렌트 정보를 추가해서 백그라운드로 파일 다운로드를 시작한다.
	s.add_torrent(p, ec);
	if (ec)
	{
		fprintf(stderr, "%s\n", ec.message().c_str());
		return 1;
	}

	char a;
	scanf("%c\n", &a);
	return 0;
}