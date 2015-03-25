#include "base.h"
#include <cstdlib>
#include <cstdio>
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = new char[suggested_size];
	buf->len = suggested_size;
}
