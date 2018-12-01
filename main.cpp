

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <string>
//#define _GLIBCXX_USE_CXX11_ABI 1


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
char* clientencoded = "ODA3ZWI1NDctYjY5My00YjE0LTgwYmItYTU4M2VlN2JiNmZjOjZmYWEzMzkwLTI5ZjgtNGNhYS04OGQzLTkyYjg2YWI4YmEyOQ==";
char* Redirect_URLs = "http://localhost";
std::string VehicleId="394C1713ED75298D38";
	static const char *postthis="grant_type=authorization_code&code=cd27a24c-058b-4f4a-85a7-2f99fe4424e5&redirect_uri=http://localhost";
int main(void)//https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/authorize?response_type=code&client_id=807eb547-b693-4b14-80bb-a583ee7bb6fc&redirect_uri=http://localhost&scope=mb:vehicle:status:general mb:user:pool:reader

{
/*
curl -X POST "https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/token" \-H "authorization: Basic ODA3ZWI1NDctYjY5My00YjE0LTgwYmItYTU4M2VlN2JiNmZjOjZmYWEzMzkwLTI5ZjgtNGNhYS04OGQzLTkyYjg2YWI4YmEyOQ" \-H "content-type: application/x-www-form-urlencoded" \-d"grant_type=authorization_code&code=bd1ffdc2-95b7-4ab9-9066-addd4eb7eda6&redirect_uri=<insert_redirect_uri_here>"
*/
//ACCESS TOKEN 
    CURLcode res,req;
    CURL *curl_handle,*get_handle;
    struct curl_slist *header= NULL,*get_header=NULL;
    FILE* out =fopen("return.txt","w");
Json::Reader reader;
    Json::Value obj;
    struct MemoryStruct chunk,get_response;

    chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    header = curl_slist_append(header,"content-type: application/x-www-form-urlencoded");


    	//char clid[200];
	std::string s,s2;
s="authorization: Basic ";
s.append(clientencoded);
    	


    header = curl_slist_append(header,s.c_str());
    res =curl_easy_setopt(curl_handle,CURLOPT_HTTPHEADER, header);
    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/token");

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
//std::cout<<clid;
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
    //printf("%s ", chunk.memory);
	reader.parse(chunk.memory,obj);
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
	std::cout<<chunk.memory<<std::endl;
	std::cout<<obj["access_token"].asString()<<std::endl;



//ACCESS TOKEN END

	get_response.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */ 

  	get_response.size = 0;

	

	get_handle = curl_easy_init();

 	req=curl_easy_setopt(get_handle,CURLOPT_HTTPHEADER, get_header);
  	// URL START
	std::string url="https://api.mercedes-benz.com/experimental/connectedvehicle/v1/vehicles/";
	url.append(VehicleId);
	url.append("/stateofcharge");
	//URL END
	//TOKEN INIT + HEADERS
	s2="authorization: Bearer "+obj["access_token"].asString();

	get_header= curl_slist_append(get_header,"accept: application/json");

	get_header= curl_slist_append(get_header,s2.c_str());

  	curl_easy_setopt(get_handle, CURLOPT_URL,url.c_str());
 	//INIT END

  	/* send all data to this function  */ 
  	curl_easy_setopt(get_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(get_handle, CURLOPT_WRITEDATA, (void *)&get_response);
 
  	/* some servers don't like requests that are made without a user-agent
     	field, so we provide one */ 
 
  	/* get it! */ 
  	req = curl_easy_perform(get_handle);
	if(req != CURLE_OK) {
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


	std::cout<<get_response.memory<<std::endl;


















    free(chunk.memory);
    fclose(out);
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return 0;
}
