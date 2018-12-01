

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <curl/curl.h>
#include <string>
#define _GLIBCXX_USE_CXX11_ABI 1


struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;

    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
char* client_id = "807eb547-b693-4b14-80bb-a583ee7bb6fc";
char* Client_Secret = "6faa3390-29f8-4caa-88d3-92b86ab8ba29";
char* Redirect_URLs = "http://localhost";
	static const char *postthis="grant_type=authorization_code&code=ae9b2850-1b7f-4311-aa14-0982f94715b0&redirect_uri=http://localhost";
int main(void)
{
    CURLcode res;
    CURL *curl_handle;
    struct curl_slist *header= NULL;
    FILE* out =fopen("return.txt","w");

    struct MemoryStruct chunk;

    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    header = curl_slist_append(header,"content-type: application/x-www-form-urlencoded");


    char clid[200];
	strcat(clid,"authorization: Bearer ");
    	strcat(clid,client_id);
	strcat(clid,":");
	strcat(clid,Client_Secret);

    header = curl_slist_append(header,clid);
    res =curl_easy_setopt(curl_handle,CURLOPT_HTTPHEADER, header);
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/token");

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    /* some servers don't like requests that are made without a user-agent
       field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, postthis);//Post field

    curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, (long)strlen(postthis));//Post size

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    }
    else {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }
    printf("%s ", chunk.memory);
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);
    fclose(out);
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return 0;
}
