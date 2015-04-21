#include "psilocin.h"

/* ps_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void ps_Panic(int i, char *prompt) {
	fprintf (stderr, prompt);
	exit(i);
}

/*	incredibly useful function that formats a string and allocates enough
	memory for fucker */
char *ps_Format(const char *format, ...) {
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


/* pulled from http://curl.haxx.se/libcurl/c/getinmemory.html */
typedef struct  {
  char *memory;
  size_t size;
} ps_Slice;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	ps_Slice *mem = (ps_Slice *) userp;

	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) ps_Panic(ps_EOALLOC, "WriteMemoryCallback: realloc failed. No more RAM, huh?");
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;	
}

/*	ps_GetHTTP - returns the body from the HTTP request. 
all the init stuff needs wrapping somewhere else */
char *ps_GetHTTP(char *url) {
	CURLcode res;

	ps_Slice slice;
	slice.memory = calloc(1, sizeof(char));
	slice.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);

	CURL *curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &slice);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "psilocin-client/0.0");
	res = curl_easy_perform(curl_handle);

	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
 		curl_easy_strerror(res));
	}
	
	curl_easy_cleanup(curl_handle);
	char *body = calloc(slice.size+1, sizeof(char));
	memcpy(body, slice.memory, slice.size);
	free(slice.memory);
		
 	curl_global_cleanup();
	return body;
}

/* couple of debug Atomizer functions from libmurp */
mp_Atomizer ps_PrintAtom(mp_Atom atom, void *p) {
	printf("Atom {\n\tDatatype:\t%d\n\tContainer:\t%d\n", atom.type, atom.container);
	if(atom.key.start) printf("\tKey:\t%.*s\n", atom.key.len, atom.key.start);
	printf("\tValue:\t%.*s\n", atom.value.len, atom.value.start);
	if ((atom.type == mp_OBJECT) || (atom.type == mp_ARRAY)) {
		int i;
		mp_ProbeSlice(&atom.value, ps_CountElems, &i);
		printf("\tElements:\t%d\n", i);
	}
	puts("\n}\n");

	// calls itself recursively
	if ((atom.type == mp_OBJECT) || (atom.type == mp_ARRAY)) mp_AtomizeSlice(&atom.value, ps_PrintAtom);
	
	if(mp_IS_ERROR(atom.type)) return mp_BREAK;
	return mp_CONTINUE;
}


mp_Atomizer ps_CountElems(mp_Atom atom, void *probe) {
	if(mp_IS_ERROR(atom.type)) return mp_BREAK;
	
	int *count = probe;
	*count += 1;

	return mp_CONTINUE;
}
