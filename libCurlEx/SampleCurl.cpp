// SampleCurl.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include<iostream>

extern "C" {
#include<curl/curl.h>
}

#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif

int main(const int argc, const char** argv){

	if (argc != 2){
		std::cerr << " Usage: ./" << argv[0] << " {url} [debug]" << std::endl;
		return 0;
	}

	const char* url = argv[1];

	// lubcURL 초기화 
	curl_global_init(CURL_GLOBAL_ALL);

	// context객체의 생성
	CURL* ctx = curl_easy_init();

	if (NULL == ctx){
		std::cerr << "Unable to initialize cURL interface" << std::endl;
		return 0;
	}

	// context 객체를 설정한다.	
	// 긁어올 url을 명시하고, url이 URL정보임을 알려준다.
	curl_easy_setopt(ctx, CURLOPT_URL, url);

	// no progress bar:
	curl_easy_setopt(ctx, CURLOPT_NOPROGRESS, 1);

	// 헤더는 표준에러로 출력하도록 하다. 
	curl_easy_setopt(ctx, CURLOPT_WRITEHEADER, stderr);


	// body 데이터는 표준출력 하도록 한다.
	curl_easy_setopt(ctx, CURLOPT_WRITEDATA, stdout);

	// context 객체의 설정 종료 


	// 웹페이지를 긁어온다. 

	const CURLcode rc = curl_easy_perform(ctx);

	if (CURLE_OK != rc){

		std::cerr << "Error from cURL: " << curl_easy_strerror(rc) << std::endl;
	}
	else{

		// get some info about the xfer:
		double statDouble;
		long statLong;
		char* statString = NULL;

		// HTTP 응답코드를 얻어온다. 
		if (CURLE_OK == curl_easy_getinfo(ctx, CURLINFO_HTTP_CODE, &statLong)){
			std::cout << "Response code:  " << statLong << std::endl;
		}

		// Content-Type 를 얻어온다.
		if (CURLE_OK == curl_easy_getinfo(ctx, CURLINFO_CONTENT_TYPE, &statString)){
			std::cout << "Content type:   " << statString << std::endl;
		}

		// 다운로드한 문서의 크기를 얻어온다.
		if (CURLE_OK == curl_easy_getinfo(ctx, CURLINFO_SIZE_DOWNLOAD, &statDouble)){
			std::cout << "Download size:  " << statDouble << "bytes" << std::endl;
		}

		// 
		if (CURLE_OK == curl_easy_getinfo(ctx, CURLINFO_SPEED_DOWNLOAD, &statDouble)){
			std::cout << "Download speed: " << statDouble << "bytes/sec" << std::endl;
		}

	}

	// cleanup
	curl_easy_cleanup(ctx);
	curl_global_cleanup();

	return(0);

}