#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "../libuv/include/uv.h"

uv_loop_t *loop;


void on_connect(uv_connect_t *req, int status)
{
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
	

	struct sockaddr_in dest;
	uv_ip4_addr(ip, port, &dest);

	uv_tcp_connect(connect, client, (const struct sockaddr*)&dest, on_connect);

	return uv_run(loop, UV_RUN_DEFAULT);
}
