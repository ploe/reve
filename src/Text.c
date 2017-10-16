#include "reve.h"

/*	incredibly useful function that formats a string and allocates enough
	memory for fucker */
rv_Text rv_TextNew(const char *format, ...) {
	va_list size_args, format_args;
  	va_start(format_args, format);
  	va_copy(size_args, format_args);

  	int size = 1 + vsnprintf(NULL, 0, format, size_args);
	va_end(size_args);
	
	char *t = calloc(size, sizeof(char));
	vsnprintf (t, size, format, format_args);
	va_end(format_args);

	return t;
}


rv_Text rv_TextFree(rv_Text t) {
	if(t) free(t);
	return NULL;
}
