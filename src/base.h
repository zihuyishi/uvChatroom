#ifndef UVCHATROOM_BASE_HEADER
#define UVCHATROOM_BASE_HEADER

#include <uv.h>

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf);

template <typename T>
void SafeDelete(T *&p) {
	if (p != nullptr) {
		delete p;
		p = nullptr;
	}
}
#endif
