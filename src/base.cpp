#include "base.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = new char[suggested_size];
	buf->len = suggested_size;
}

void PrintErrorMsg(const char *msg) {
	write(STDERR_FILENO, msg, strlen(msg));
}

size_t str_cleanTrail(char *str) {
    auto len = strlen(str);
    long i = len - 1;
    while (i != -1 && (str[i] == '\n' || str[i] == '\r' || str[i] == ' ' || str[i] == '\t'))
        i--;
    str[i+1] = 0;
    return (size_t) (i+1);
}
