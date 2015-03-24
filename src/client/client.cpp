#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <assert.h>
#include <iostream>
#include "../../include/base.h"

uv_loop_t *loop;

typedef struct connect_info_s {
	uv_tcp_t *p_tcp;
} connect_info_t;

void read_message(connect_info_t* info);


void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
	std::cout << "server say : " << buf->base << std::endl;
	if (buf->base) {
		free(buf->base);
	}
}

void on_send_name(uv_write_t *req, int status)
{
	connect_info_t *info = reinterpret_cast<connect_info_t*>(req->data);
	free(req);
	if (status < 0) {
		std::cout << "send name failed" << std::endl;
	} else {
		std::cout << "send name succeed" << std::endl;
		assert(info && "connect_info is null");
		read_message(info);
	}
}

void on_send_message(uv_write_t *write, int status)
{
	if (status < 0) {
		std::cout << "send message fail" << std::endl;
	} else {
		std::cout << "send message succeed\n";
	}
	connect_info_t *info = reinterpret_cast<connect_info_s*>(write->data);
	free(write);
}

char *get_message()
{
	std::cout << "enter message:\n";
	char *buffer = new char[4196];
	char c;
	size_t pos = 0;
	c = getchar();
	while (c != '\n' && c != EOF)
	{
		buffer[pos] = c;
		if (pos == 4195)
			break;
		++pos;
		c = getchar();
	}
	buffer[pos] = 0;
	return buffer;
}

void read_message_thread(void *arg)
{
	connect_info_t *info = reinterpret_cast<connect_info_t*>(arg);
	char *buffer = get_message(); 
	size_t len = strlen(buffer);
	uv_buf_t wrBuf = uv_buf_init(buffer, len+1);
	
	uv_write_t *write = new uv_write_t;
	write->data = info;
	
	uv_write(write, reinterpret_cast<uv_stream_t*>(info->p_tcp), &wrBuf, 1, on_send_message);	
	delete[] buffer;
	read_message(info);
}
void read_message(connect_info_t *info)
{
	uv_thread_t hThread;
	uv_thread_create(&hThread, read_message_thread, info);
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
	
		uv_write_t* write = (uv_write_t*)malloc(sizeof(uv_write_t));	
		write->data = info;

		char *buffer = get_message();
		size_t len = strlen(buffer);
		uv_buf_t wrBuf = uv_buf_init(buffer, len+1);
		uv_write(write, (uv_stream_t*)client, &wrBuf, 1, on_send_name);
		delete[] buffer;
	}
		
}

void print_usage(const char *fileName)
{
	std::cout << "usage :\n" << fileName << " ip port\n";
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
