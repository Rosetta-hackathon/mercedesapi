

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <time.h>
//#define _GLIBCXX_USE_CXX11_ABI 1
#define DEBUG 1
class Vehicle{

public :
Vehicle(std::string s,float la,float lon,bool c=false):VehicleID(s),latitude(la),longtitude(lon),consent(c),SOC(0){}
void Get_auth();

void Update_auth();

std::string AUTH_KEY;

std::string VehicleID;

float SOC;

float latitude;

float longtitude;

bool consent;
};
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
//std::string VehicleId="394C1713ED75298D38";
	static const char *postthis="grant_type=authorization_code&code=1c8ca109-38d5-42d0-b210-1201cd93843e&redirect_uri=http://localhost";
int main(void){
//https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/authorize?response_type=code&client_id=807eb547-b693-4b14-80bb-a583ee7bb6fc&redirect_uri=http://localhost&scope=mb:vehicle:status:general mb:user:pool:reader


/*
curl -X POST "https://api.secure.mercedes-benz.com/oidc10/auth/oauth/v2/token" \-H "authorization: Basic ODA3ZWI1NDctYjY5My00YjE0LTgwYmItYTU4M2VlN2JiNmZjOjZmYWEzMzkwLTI5ZjgtNGNhYS04OGQzLTkyYjg2YWI4YmEyOQ" \-H "content-type: application/x-www-form-urlencoded" \-d"grant_type=authorization_code&code=bd1ffdc2-95b7-4ab9-9066-addd4eb7eda6&redirect_uri=<insert_redirect_uri_here>"
*/
//ACCESS TOKEN 

	std::vector<Vehicle> Vehicles;

    	CURLcode res,req,req2;

    	CURL *curl_handle;

    	struct curl_slist *header= NULL;

    	FILE* out =fopen("return.json","w");

	Json::Reader reader,reader2,reader3;

    	Json::Value obj,obj2,obj3;

    	struct MemoryStruct chunk,get_response,get_response2;

	Vehicle temp("394C1713ED75298D38",0,0,false);

	Vehicles.push_back(temp);

    	chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */

    	chunk.size = 0;    /* no data at this point */

    	curl_global_init(CURL_GLOBAL_ALL);

    	/* init the curl session */
    	curl_handle = curl_easy_init();

    	header = curl_slist_append(header,"content-type: application/x-www-form-urlencoded");


	std::string s,s2,s1;
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
         */
	#if DEBUG
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);

	#endif
    }
    //printf("%s ", chunk.memory);
	reader.parse(chunk.memory,obj);
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
	std::cout<<chunk.memory<<std::endl;
	std::cout<<obj["access_token"].asString()<<std::endl;
	Vehicles[0].AUTH_KEY=obj["access_token"].asString();
	std::string url;

//ACCESS TOKEN END
/*f

*/	
	while(1){
	CURL *get_handle,*get_handle2;

    	struct curl_slist *get_header=NULL,*get_header2=NULL;
	get_response.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */ 

  	get_response.size = 0;
	get_response2.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */ 

  	get_response2.size = 0;

	for(auto vehicle : Vehicles){
	if(vehicle.AUTH_KEY!=""){
	get_handle = curl_easy_init();

 	
  	// URL START
	url="https://api.mercedes-benz.com/experimental/connectedvehicle/v1/vehicles/";
	url.append(vehicle.VehicleID);
	url.append("/stateofcharge");
	//URL END
	//TOKEN INIT + HEADERS
	#if DEBUG
	std::cout<<url<<std::endl;
	#endif
	s1="authorization: Bearer "+vehicle.AUTH_KEY;

	get_header= curl_slist_append(get_header,"accept: application/json");
	get_header= curl_slist_append(get_header,"Access-Control-Allow-Headers: *");
	//get_header= curl_slist_append(get_header,"Content-Type");
	get_header= curl_slist_append(get_header,s1.c_str());
	#if DEBUG
	std::cout<<s2<<std::endl;
	#endif
  	curl_easy_setopt(get_handle, CURLOPT_URL,url.c_str());
 	//INIT END
	req=curl_easy_setopt(get_handle,CURLOPT_HTTPHEADER, get_header);
  	/* send all data to th	is function  */ 
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
	#if DEBUG
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
	#endif
    	}

	curl_easy_cleanup(get_handle);
	#if DEBUG
	std::cout<<get_response.memory<<std::endl;
	#endif
	reader.parse(get_response.memory,obj2);
	//reader2.parse(get_response,obj2);

	vehicle.SOC=obj2["stateofcharge"]["value"].asFloat();
	#if DEBUG
	
	std::cout<<vehicle.SOC;
	
	std::cout<<std::endl;
	#endif
	sleep(1);
	//*********************************LOCATION*********************************************
	get_handle2 = curl_easy_init();

 	
  	// URL START
	url="https://api.mercedes-benz.com/experimental/connectedvehicle/v1/vehicles/";
	url.append(vehicle.VehicleID);
	url.append("/location");
	//URL END
	//TOKEN INIT + HEADERS
	#if DEBUG
	std::cout<<url<<std::endl;
	#endif
	s2="authorization: Bearer "+vehicle.AUTH_KEY;

	get_header2= curl_slist_append(get_header2,"accept: application/json");
	get_header2= curl_slist_append(get_header2,"Access-Control-Allow-Headers: *");
	//get_header2= curl_slist_append(get_header2,"Content-Type");
	get_header2= curl_slist_append(get_header2,s2.c_str());
	#if DEBUG
	std::cout<<s2<<std::endl;
	#endif
  	curl_easy_setopt(get_handle2, CURLOPT_URL,url.c_str());
 	//INIT END
	req2=curl_easy_setopt(get_handle2,CURLOPT_HTTPHEADER, get_header2);
  	/* send all data to th	is function  */ 
  	curl_easy_setopt(get_handle2, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  	/* we pass our 'chunk' struct to the callback function */ 
  	curl_easy_setopt(get_handle2, CURLOPT_WRITEDATA, (void *)&get_response2);
 
  	/* some servers don't like requests that are made without a user-agent
     	field, so we provide one */ 
 
  	/* get it! */ 
  	req2 = curl_easy_perform(get_handle2);
	if(req2 != CURLE_OK) {
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
	#if DEBUG
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
	#endif
    	}

	curl_easy_cleanup(get_handle2);
	#if DEBUG
	std::cout<<get_response2.memory<<std::endl;
	#endif
	reader.parse(get_response2.memory,obj3);
	//reader2.parse(get_response,obj2);

	vehicle.SOC=obj3["stateofcharge"]["value"].asFloat();
	#if DEBUG
	
	std::cout<<vehicle.SOC;
	
	std::cout<<std::endl;
	#endif


	curl_slist_free_all(get_header);
	curl_slist_free_all(get_header2);

	}
	
	}

	

	











    
    
	sleep(5);
	}
free(chunk.memory);free(get_response.memory);
    /* we're done with libcurl, so clean it up */
    curl_global_cleanup();

    return 0;
}
