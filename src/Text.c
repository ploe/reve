#include "reve.h"

/*	incredibly useful function that formats a string and allocates enough
	memory for fucker */
rv_Text rv_TextNew(const char *format, ...) {
	va_list size_args, format_args;
	va_start(format_args, format);
	va_copy(size_args, format_args);

	int size = 1 + vsnprintf(NULL, 0, format, size_args);
	va_end(size_args);

	rv_Text t = calloc(size, sizeof(char));
	vsnprintf (t, size, format, format_args);
	va_end(format_args);

	return t;
}

rv_Text rv_TextFromFile(const char *path) {
	FILE *file = fopen(path, "r");
	if (!file) rv_Panic(rv_ELOAD, "unable to load shader");

	long size = 0;
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);

	rv_Text buffer = calloc(size+1, sizeof(char));
	if (!buffer) rv_Panic(rv_ELOAD, "unable to allocate shader src");

	if(fread(buffer, 1, size, file) != size) rv_Panic(rv_ELOAD, "unable to read file in to memory");

	fclose(file);
	return buffer;
}

rv_Text rv_TextFree(rv_Text t) {
	if(t) free(t);
	return NULL;
}
