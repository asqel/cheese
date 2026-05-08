#include "../../include/cheese.h"
#include <dlfcn.h>

extern void *libpiss_handler;

void *retrieve_function(char *func_name)
{
	void *function = dlsym(libpiss_handler, func_name);
	char *error = dlerror();

	if (error != NULL) {
		fprintf(stderr, "Error: Function `%s' not found in %s\n", func_name, LIBPISS);
		return (NULL);
	}
	return (function);
}
