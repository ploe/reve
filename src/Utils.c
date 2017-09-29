#include "reve.h"

/* rv_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void rv_Panic(int i, const char *prompt) {
	fprintf (stderr, "%s\n", prompt);
	exit(i);
}

/*	incredibly useful function that formats a string and allocates enough
	memory for fucker */
char *rv_Format(const char *format, ...) {
	va_list size_args, format_args;
  	va_start(format_args, format);
  	va_copy(size_args, format_args);

  	int size = 1 + vsnprintf(NULL, 0, format, size_args);
	va_end(size_args);
	
	char *tmp = calloc(size, sizeof(char));
	vsnprintf (tmp, size, format, format_args);
	va_end(format_args);

	return tmp;
}
