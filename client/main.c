#include "psilocin.h"
#include <curl/curl.h>

/* ps_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void ps_Panic(int i, char *prompt) {
	fprintf (stderr, prompt);
	exit(i);
}

typedef struct  {
  char *memory;
  size_t size;
} ps_Slice;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	ps_Slice *mem = (ps_Slice *) userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) ps_Panic(ps_EOALLOC, "WriteMemoryCallback: realloc failed. No more RAM, huh?");
	printf("%d\n", realsize);
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	printf("%d\n", mem->size);
	mem->memory[mem->size] = 0;

	return realsize;	
}

int main() {
	CURLcode res;

	ps_Slice slice;
	slice.memory = calloc(1, sizeof(char));
	slice.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, "http://arsethat.co.uk");
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &slice);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "psilocin-client/0.0");
	res = curl_easy_perform(curl_handle);

	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
 		curl_easy_strerror(res));
	}

	curl_easy_cleanup(curl_handle); 
	free(slice.memory);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();
//	ps_Crew c;
//	ps_STAGE(&c);
//	ps_CrewNew(ps_STAGE);
//	while (ps_CrewRoll()) continue;
	return 0;
}
