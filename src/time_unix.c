#include "cheese.h"
#if defined(__linux__) || defined(__APPLE)
#include <sys/time.h>

uint64_t cheese_get_time() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return t.tv_sec * 1000 + t.tv_nsec * 1000000 - time_start;
}

#endif
