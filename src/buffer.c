#include "cheese.h"

#define BUFFER_INC 256U

int	buffer_append(buffer_t *buffer, void *data, size_t len) {
	if (!buffer)
		return 1;
	if (buffer->alloc_len < buffer->len + len) {
		size_t alloc_len = buffer->len + len;
		if (alloc_len % BUFFER_INC)
			alloc_len = (alloc_len / BUFFER_INC + 1) * BUFFER_INC;

		void *new_data = realloc(buffer->data, alloc_len);
		if (!new_data)
			return 1;

		buffer->data = new_data;
		buffer->alloc_len = alloc_len;
	}
	memcpy(&buffer[buffer->len], data, len);
	buffer->len += len;
	return 0;
}

void buffer_remove(buffer_t *buffer, size_t start, size_t len) {
	if (!buffer)
		return ;

	int end = start + len;
	uint8_t *data = buffer->data;
	for (size_t i = end; i < buffer->len; i++)
		data[i - end] = data[i];
	
	buffer->len -= len;
	size_t alloc_len = buffer->len;
	if (alloc_len % BUFFER_INC)
		alloc_len = (alloc_len / BUFFER_INC + 1) * BUFFER_INC;
	if (alloc_len < buffer->alloc_len) {
		void *data = realloc(buffer->data, alloc_len);
		if (!data)
			return ;
		buffer->alloc_len = alloc_len;
		buffer->data = data;
	}
}

void buffer_free(buffer_t *buffer) {
	if (!buffer)
		return ;
	free(buffer->data);
	buffer->len = 0;
	buffer->alloc_len = 0;
}
