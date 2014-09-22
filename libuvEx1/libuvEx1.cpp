// libuvEx1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define TEST_PORT 9000

#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Iphlpapi.lib")



#define log(x) printf("%s\n", x);

uv_loop_t *loop;

void on_connect(uv_connect_t *req, int status);
void on_write_end(uv_write_t *req, int status);
uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size);
void echo_read(uv_stream_t *server, ssize_t nread, uv_buf_t buf);

// サーバからのレスポンスを表示
void echo_read(uv_stream_t *server, ssize_t nread, uv_buf_t buf) {
	if (nread == -1) {
		fprintf(stderr, "error echo_read");
		return;
	}

	// 結果を buf から取得して表示
	printf("result: %s\n", buf.base);
}

// suggeseted_size で渡された領域を確保
uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size) {
	// 読み込みのためのバッファを、サジェストされたサイズで確保
	return uv_buf_init((char*)malloc(suggested_size), suggested_size);
}

// サーバへデータ送信後, サーバからのレスポンスを読み込む
void on_write_end(uv_write_t *req, int status) {
	if (status == -1) {
		fprintf(stderr, "error on_write_end");
		return;
	}

	// 書き込みが終わったら、すぐに読み込みを開始
	uv_read_start(req->handle, (uv_alloc_cb)alloc_buffer, (uv_read_cb)echo_read);
}

// サーバとの接続を確立後, サーバに文字列を送信
void on_connect(uv_connect_t *req, int status) {
	if (status == -1) {
		fprintf(stderr, "error on_write_end");
		return;
	}

	// 送信メッセージを登録
	char *message = "hello.txt";
	int len = strlen(message);

	/** これだとセグフォ
	* uv_buf_t buf[1];
	* buf[0].len = len;
	* buf[0].base = message;
	*/

	// 送信データ用のバッファ
	char buffer[100];
	uv_buf_t buf = uv_buf_init(buffer, sizeof(buffer));

	// 送信データを載せる
	buf.len = len;
	buf.base = message;

	// ハンドルを取得
	uv_stream_t* tcp = req->handle;

	// 書き込み用構造体
	uv_write_t write_req;

	int buf_count = 1;
	// 書き込み
	uv_write(&write_req, tcp, &buf, buf_count, on_write_end);
}

int main(void) {
	// loop 生成
	loop = uv_default_loop();

	// Network I/O の構造体
	uv_tcp_t client;

	// loop への登録
	uv_tcp_init(loop, &client);

	// アドレスの取得
	struct sockaddr_in req_addr;
	uv_ip4_addr("127.0.0.1", 9000, &req_addr);

	// TCP コネクション用の構造体
	uv_connect_t connect_req;

	// 接続
	uv_tcp_connect(&connect_req, &client, (const sockaddr*)&req_addr, on_connect);

	// ループを開始
	return uv_run(loop, UV_RUN_DEFAULT);
}