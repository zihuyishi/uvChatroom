#include "../include/base.h"
#include <cstdlib>
void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}
