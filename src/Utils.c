#include "reve.h"

/* rv_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void rv_Panic(int i, const char *prompt) {
	fprintf (stderr, "%s\n", prompt);
	exit(i);
}


