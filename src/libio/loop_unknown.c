#include "libio.h"

#if !defined(_WIN32) && !defined(__linux__) && !defined(__APPLE)
#error "unsuported OS " __FILE__
#endif
