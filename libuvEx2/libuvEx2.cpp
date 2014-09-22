// libuvEx2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "uv.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TEST_PORT 9000

#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "Iphlpapi.lib")


typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req_t;

static uv_loop_t* loop;

static int server_closed;
static uv_tcp_t tcpServer;
static uv_handle_t* server;

static void after_write(uv_write_t* req, int status);
static void after_read(uv_stream_t*, ssize_t nread, const uv_buf_t* buf);
static void on_close(uv_handle_t* peer);
static void on_server_close(uv_handle_t* handle);
static void on_connection(uv_stream_t*, int status);


static void after_write(uv_write_t* req, int status) {
	write_req_t* wr;

	/* Free the read/write buffer and the request */
	wr = (write_req_t*)req;
	free(wr->buf.base);

	if (status == 0) {
		free(wr);
		return;
	}

	fprintf(stderr,
		"uv_write error: %s - %s\n",
		uv_err_name(status),
		uv_strerror(status));

	if (!uv_is_closing((uv_handle_t*)req->handle))
		uv_close((uv_handle_t*)req->handle, on_close);
	free(wr);
}


static void after_read(uv_stream_t* handle,
	ssize_t nread,
	const uv_buf_t* buf) {
	int i;
	write_req_t *wr;

	if (nread < 0) {
		/* Error or EOF */
		//assert(nread == UV_EOF);

		if (buf->base) {
			free(buf->base);
		}

		uv_close((uv_handle_t*)handle, on_close);
		return;
	}

	if (nread == 0) {
		/* Everything OK, but nothing read. */
		free(buf->base);
		return;
	}

	/*
	* Scan for the letter Q which signals that we should quit the server.
	* If we get QS it means close the stream.
	*/
	if (!server_closed) {
		for (i = 0; i < nread; i++) {
			if (buf->base[i] == 'Q') {
				if (i + 1 < nread && buf->base[i + 1] == 'S') {
					free(buf->base);
					uv_close((uv_handle_t*)handle, on_close);
					return;
				}
				else {
					uv_close(server, on_server_close);
					server_closed = 1;
				}
			}
		}
	}

	wr = (write_req_t*)malloc(sizeof *wr);
	assert(wr != NULL);
	wr->buf = uv_buf_init(buf->base, nread);

	if (uv_write(&wr->req, handle, &wr->buf, 1, after_write)) {
		assert(0);
	}
}


static void on_close(uv_handle_t* peer) {
	free(peer);
}


static void echo_alloc(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}


static void on_connection(uv_stream_t* server, int status) {
	uv_stream_t* stream;
	int r;

	if (status != 0) {
		fprintf(stderr, "Connect error %s\n", uv_err_name(status));
	}
	assert(status == 0);

	stream = (uv_stream_t *)malloc(sizeof(uv_tcp_t));
	assert(stream != NULL);
	r = uv_tcp_init(loop, (uv_tcp_t*)stream);
	assert(r == 0);

	/* associate server with stream */
	stream->data = server;

	r = uv_accept(server, stream);
	assert(r == 0);

	r = uv_read_start(stream, echo_alloc, after_read);
	assert(r == 0);
}


static void on_server_close(uv_handle_t* handle) {
	assert(handle == server);
}


static void on_send(uv_udp_send_t* req, int status);


static void on_recv(uv_udp_t* handle,
	ssize_t nread,
	const uv_buf_t* rcvbuf,
	const struct sockaddr* addr,
	unsigned flags) {
	uv_udp_send_t* req;
	uv_buf_t sndbuf;

	assert(nread > 0);
	assert(addr->sa_family == AF_INET);

	req = (uv_udp_send_t *)malloc(sizeof(*req));
	assert(req != NULL);

	sndbuf = *rcvbuf;
	assert(0 == uv_udp_send(req, handle, &sndbuf, 1, addr, on_send));
}


static void on_send(uv_udp_send_t* req, int status) {
	assert(status == 0);
	free(req);
}


static int tcp4_echo_start(int port) {
	struct sockaddr_in addr;
	int r;

	assert(0 == uv_ip4_addr("0.0.0.0", port, &addr));

	server = (uv_handle_t*)&tcpServer;
	
	r = uv_tcp_init(loop, &tcpServer);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Socket creation error\n");
		return 1;
	}

	r = uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr, 0);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Bind error\n");
		return 1;
	}

	r = uv_listen((uv_stream_t*)&tcpServer, SOMAXCONN, on_connection);
	if (r) {
		/* TODO: Error codes */
		fprintf(stderr, "Listen error %s\n", uv_err_name(r));
		return 1;
	}

	return 0;
}


int main(void) {
	
	loop = uv_default_loop();

	if (tcp4_echo_start(TEST_PORT))
		return 1;

	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}