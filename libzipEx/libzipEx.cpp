// libzipEx.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "zip.h"

#pragma comment(lib, "zip.lib")

int main()
{
	//압축된 파일 열기
	int err = 0;
	zip *z = zip_open("foo.zip", 0, &err);

	//압축 파일내에 주어진 파일이 존재하는지 확인
	//파일 이름은 대소문자를 정확히 넣어주어야 한다.
	const char *name = "ReadMe.txt";
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	//찾고자 하는 파일의 크기 만큼 메모리 할당
	char *contents = new char[st.size];

	//찾고자 하는 파일의 포인터를 얻어서 파일의 내용을 메모리에 카피
	zip_file *f = zip_fopen(z, "ReadMe.txt", 0);
	zip_fread(f, contents, st.size);
	zip_fclose(f);

	//라이브러리를 종료한다.
	zip_close(z);
}