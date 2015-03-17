#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "../include/base.h"

uv_loop_t *loop;

typedef struct connect_info_s {
	uv_tcp_t *p_tcp;
} connect_info_t;

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
	std::cout << "server say : " << buf->base << std::endl;
	if (buf->base) {
		free(buf->base);
	}
}

void on_write(uv_write_t *req, int status)
{
	if (status < 0) {
		std::cout << "send name failed" << std::endl;
	} else {
		std::cout << "send name succeed" << std::endl;
	}
	free(req);
}

void on_connect(uv_connect_t *req, int status)
{
	connect_info_t *info = (connect_info_t*)req->data;
	assert(info);
	uv_tcp_t *client = info->p_tcp;
	assert(client);
	if (status < 0) {
		std::cout << "connect fail " << uv_strerror(status) << std::endl;
		if (info) {
			if (info->p_tcp)
				free(info->p_tcp);
			free(info);
		}
		free(req);
	} else {
		uv_read_start((uv_stream_t*)client, alloc_buffer, on_read);

		std::cout << "please enter your name:\n";
		std::string userName;
		std::cin >> userName;
	
		uv_write_t* write = (uv_write_t*)malloc(sizeof(uv_write_t));	
		char *buffer = new char[userName.length()+1];
		strcpy(buffer, userName.c_str());	
		uv_buf_t wrBuf = uv_buf_init(buffer, userName.length());
		delete[] buffer;	
		uv_write(write, (uv_stream_t*)client, &wrBuf, 1, on_write);
	}
		
}

void print_usage(const char *fileName)
{
	printf("usage : \n%s ip port\n", fileName);
}
int main(int argc, char **argv)
{
	if (argc != 3) {
		print_usage(argv[0]);
		return 1;
	}

	const char *ip = argv[1];
	const int port = atoi(argv[2]);

	loop = uv_default_loop();
	
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);

	uv_connect_t *connect = (uv_connect_t *)malloc(sizeof(uv_connect_t));
	connect_info_t *info = (connect_info_t *)malloc(sizeof(connect_info_t));
	info->p_tcp = client;
	connect->data = info;


	struct sockaddr_in dest;
	uv_ip4_addr(ip, port, &dest);

	uv_tcp_connect(connect, client, (const struct sockaddr*)&dest, on_connect);

	return uv_run(loop, UV_RUN_DEFAULT);
}
