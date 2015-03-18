#include <cstdlib>
#include <iostream>
#include <cstring>
#include "../../include/base.h"

#define DEFAULT_BACKLOG 128

uv_loop_t *loop;

void on_write(uv_write_t* write, int status)
{
	free(write);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	//接受客户端信息
	std::cout << "receive client message " << buf->base << std::endl;

	uv_write_t *write = (uv_write_t*)malloc(sizeof(uv_write_t));
	char buffer[] = "hello, this is a chatroom server";
	uv_buf_t wrBuf = uv_buf_init(buffer, strlen(buffer));
	
	uv_write(write, (uv_stream_t*)client, &wrBuf, 1, on_write);

	if (buf->base)
		free(buf->base);
}
void on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0) {
		std::cout << "New connection error " << uv_strerror(status) << std::endl;
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
	std::cout << "usage :\n" << fileName << " PORT\n";
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
		std::cout << "Listen error " << uv_strerror(r) << std::endl;
		return 1;
	} else {
		std::cout << "succeed, listen on " << port << std::endl;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}