#include "psilocin.h"

/* ps_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void ps_Panic(int i, char *prompt) {
	fprintf (stderr, prompt);
	exit(i);
}

int main() {
	ps_Crew c;
	ps_STAGE(&c);
	ps_CrewNew(ps_STAGE);
	while (ps_CrewRoll()) continue;
	return 0;
}
