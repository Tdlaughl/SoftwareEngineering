#include <iostream>
#include <curl/curl.h>

// Define Supabase API credentials
const std::string SUPABASE_URL = "https://xjvowcvengujotdcwbjm.supabase.co";
const std::string SUPABASE_API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inhqdm93Y3Zlbmd1am90ZGN3YmptIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MDc3Nzg4NzksImV4cCI6MjAyMzM1NDg3OX0.eEVskzeaC4ZKqL_GbGkJZzC2WNb01VrKnsKtKNZVm94";

// Callback function to write response data
size_t writeCallback(void *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append((char *)ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // Construct the URL for a simple query
        std::string queryUrl = SUPABASE_URL + "/rest/v1/Bruh";

        // Set up the request headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + SUPABASE_API_KEY).c_str());

        // Set the URL and request headers
        curl_easy_setopt(curl, CURLOPT_URL, queryUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);


        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            // Response received, print it
            std::cout << "Response: " << readBuffer << std::endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    } else {
        std::cerr << "Failed to initialize libcurl" << std::endl;
    }

    // Cleanup libcurl resources
    curl_global_cleanup();

    return 0;
}
