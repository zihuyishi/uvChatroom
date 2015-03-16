#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "../libuv/include/uv.h"

#define DEFAULT_BACKLOG 128

uv_loop_t *loop;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	//接受客户端信息
	printf("receive client message %s\n", buf->base);

	if (buf->base)
		free(buf->base);
}
void on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0) {
		fprintf(stderr, "New connection error %s\n", uv_strerror(status));
		return ;
	}
	
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		//连接成功
		uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}	
}

void print_usage(const char *fileName)
{
	printf("usage :\n %s PORT \n", fileName);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		print_usage(argv[0]);	
		return 1;
	}

	int port = atoi(argv[1]);	

	loop = uv_default_loop();
	
	uv_tcp_t server;
	uv_tcp_init(loop, &server);
	
	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", port, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_strerror(r));
		return 1;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}
