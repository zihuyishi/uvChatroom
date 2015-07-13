#include <cstdlib>
#include <iostream>
#include <vector>
#include <base.h>

static const int DEFAULT_BACKLOG = 128;
static const int MAX_USERNAME = 100;

uv_loop_t *loop;

typedef struct {
    uv_tcp_t *connection;
    char *username;
    void *data;
} connection_info_t;

std::vector<connection_info_t *> connectionList;

static void write_cb(uv_write_t *write, int status)
{
    char *buffer = reinterpret_cast<char *>(write->data);
	SafeDeletes(buffer);
	SafeDelete(write);
}

static void close_cb(uv_handle_t *client) {
    connection_info_t *info = (connection_info_t *) client->data;
    if (info) {
        SafeDeletes(info->username);
        SafeDelete(info);
    }
    SafeDelete(client);
}
static void shutdown_cb(uv_shutdown_t* req, int status) {
    uv_close((uv_handle_t*)req->handle, close_cb);
    delete req;
}

static void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	//接受客户端信息
	uv_tcp_t *connection = (uv_tcp_t*)client;
    connection_info_t *info = (connection_info_t *) connection->data;
    if (nread < 0) {
        //Errors or EOF
        delete[] buf->base;
        auto iter = connectionList.begin();
        for (; iter != connectionList.end(); ++iter) {
            if (*iter == info) break;
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
    buf->base[nread] = 0;
    str_cleanTrail(buf->base);
    if (info->username == nullptr) { //user name msg
        info->username = new char[nread];
        strncpy(info->username, buf->base, (size_t) nread);
        std::cout << info->username << " has join in.\n";
    } else {
        std::cout << "receive client message :\n" << buf->base << std::endl;
        for (auto c : connectionList)
        {
            if (c == info) continue;

            char *buffer = new char[nread + 256];
            std::string msg = info->username;
            msg.append(" says: ");
            msg.append(buf->base);
            msg.append("\n");

            strncpy(buffer, msg.c_str(), (size_t) (nread+256));
            uv_write_t *write = new uv_write_t;
            write->nbufs = 1;
            write->bufs = new uv_buf_t[write->nbufs];
            write->bufs[0] = uv_buf_init(buffer, (unsigned int) strlen(buffer));
            write->data = buffer;
            uv_write(write, (uv_stream_t *) c->connection, write->bufs, write->nbufs, write_cb);
        }
    }
    delete[] buf->base;

}
static void askname_cb(uv_write_t *write, int status)
{
    SafeDelete(write);
    if (status < 0) {
        PrintErrorMsg("ask username failed");
    }
}
static void ask_username(connection_info_t *connectionInfo) {
    uv_write_t *write = new uv_write_t;
    write->data = connectionInfo;
    const char *ask = "Pelease enter your name\n";
    uv_buf_t buf = uv_buf_init(const_cast<char *>(ask), (unsigned int) strlen(ask));

    uv_read_start((uv_stream_t*)connectionInfo->connection, alloc_buffer, echo_read);

    uv_write(write, (uv_stream_t *)connectionInfo->connection, &buf, 1, askname_cb);
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
        connection_info_t *connectionInfo = new connection_info_t;
        connectionInfo->connection = client;
        connectionInfo->username = nullptr;
        client->data = connectionInfo;
		connectionList.push_back(connectionInfo);

        ask_username(connectionInfo);
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
