#ifndef UVCHATROOM_BASE_HEADER
#define UVCHATROOM_BASE_HEADER

#include <uv.h>
#include "user.h"
#include "message.h"

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t *buf);

/**
 * safe delete point
 */
template <typename T>
void SafeDelete(T *&p) {
	if (p != nullptr) {
		delete p;
		p = nullptr;
	}
}
/**
 * safe delete point array
 */
template <typename T>
void SafeDeletes(T *&p) {
	if (p != nullptr) {
		delete[] p;
		p = nullptr;
	}
}

void PrintErrorMsg(const char *msg);


size_t str_cleanTrail(char *str);
#endif
