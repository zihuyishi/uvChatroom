#include <cstdlib>
#include <iostream>
#include <vector>
#include <base.h>

#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
std::vector<uv_tcp_t*> connectionList;

void on_write(uv_write_t* write, int status)
{
	SafeDelete(write);
}

static void close_cb(uv_handle_t *client) {
    delete client;
}
static void shutdown_cb(uv_shutdown_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, close_cb);
    delete req;
}

static void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	//接受客户端信息
	std::cout << "receive client message :\n" << buf->base << std::endl;

	uv_tcp_t *connection = (uv_tcp_t*)client;
    if (nread < 0) {
        //Errors or EOF
        delete[] buf->base;
        auto iter = connectionList.begin();
        for (; iter != connectionList.end(); ++iter) {
            if (*iter == connection) break;
        }
        if (iter != connectionList.end()) {
            connectionList.erase(iter);
        }
        uv_shutdown_t *shutdown_req = new uv_shutdown_t;
        if (uv_shutdown(shutdown_req, client, shutdown_cb) != 0) {

        }
        return ;
    }
    if (nread == 0) {
        delete[] buf->base;
        return ;
    }
	char *buffer = new char[nread+1];
	strcpy(buffer, buf->base);
	uv_buf_t wrBuf = uv_buf_init(buffer, strlen(buffer)+1);
	for (auto c : connectionList)
	{
		if (c == connection) continue;
		uv_write_t *write = new uv_write_t;	
		uv_write(write, (uv_stream_t*)c, &wrBuf, 1, on_write);
	}	
	delete [] buffer;
	if (buf->base)
		delete[] buf->base;

}
static void on_new_connection(uv_stream_t *server, int status)
{
	if (status < 0) {
		std::cout << "New connection error " << uv_strerror(status) << std::endl;
		return ;
	}
	
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(loop, client);
	if (uv_accept(server, (uv_stream_t*)client) == 0) {
		//连接成功
        client->data = server;
		connectionList.push_back(client);
		uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
	} else {
		uv_close((uv_handle_t*)client, NULL);
	}
}

static void print_usage(const char *fileName)
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
