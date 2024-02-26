// #ifndef supa_h
// #define supa_h

// #include <iostream>
// #include <nlohmann/json.hpp>
// #include <curl/curl.h>
// #include <vector>

// using namespace std;

// class Database
// {
//     public:
//         Database();
//         const string SUPABASE_URL = "https://xjvowcvengujotdcwbjm.supabase.co";
//         const string SUPABASE_API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inhqdm93Y3Zlbmd1am90ZGN3YmptIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MDc3Nzg4NzksImV4cCI6MjAyMzM1NDg3OX0.eEVskzeaC4ZKqL_GbGkJZzC2WNb01VrKnsKtKNZVm94";
//         void addRow();
//         static size_t writeCallback(void* ptr, size_t size, size_t nmemb, string* data);
//         void displayDatabase(string& readBuffer);
//         void decodeJSON(const std::string& jsonString, std::vector<std::string>& codenames); 
//         int supa(); 
// };




//I didn't wish to change much of anything from Trenton's work, I tried to keep
//any adjustments minimal, and I only touched code within decodeJSON to get the IDs.


#include "supa.h"

Database::Database()
{

}

// Define Supabase API credentials

// Callback function to write response data
size_t Database::writeCallback(void* ptr, size_t size, size_t nmemb, string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

void Database::addRow()
{

    CURL* curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {

        int idNumber = 0;
        string codeName = "";

        cout << "Enter the ID number: ";
        cin >> idNumber;
        cout << endl << "Enter the Codename: ";
        cin >> codeName;
        // Construct the URL for inserting data into the `test` table
        string endpoint = SUPABASE_URL + "/rest/v1/Bruh";

        // Set up the request headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("apikey: " + SUPABASE_API_KEY).c_str());

        // Set the URL and request headers
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the request body (data to be inserted into the table)
        string requestBody = "{\"id\": " + to_string(idNumber) + ", \"codename\": \"" + codeName + "\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            cout << "Data inserted successfully" << endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    else {
        cerr << "Failed to initialize libcurl" << endl;
    }

    // Cleanup libcurl resources
    curl_global_cleanup();
}
void Database::displayDatabase(string& readBuffer) {
    CURL* curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        // Construct the URL for retrieving data from the `test` table
        string endpoint = SUPABASE_URL + "/rest/v1/Bruh";

        // Set up the request headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("apikey: " + SUPABASE_API_KEY).c_str());

        // Set the URL and request headers
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback function to handle response data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        }
        else {
            // Response received, print it
            cout << "Response: " << readBuffer << endl;
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    else {
        cerr << "Failed to initialize libcurl" << endl;
    }

    // Cleanup libcurl resources
    curl_global_cleanup();
}
using json = nlohmann::json;

void Database::decodeJSON(const std::string& jsonString, std::vector<std::string>& codenames) {
    try {
        // Parse JSON string
        auto jsonArray = json::parse(jsonString);

        // Iterate over each object in the array
        for (const auto& j : jsonArray) {
            // Extract the codename from each object and add it to the vector
            codenames.push_back(j["codename"].get<std::string>());
            ids.push_back(j["id"].get<int>());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}


int Database::supa() {
    int choice;

    cout << "Choose an option:" << endl;
    cout << "1. Add a row to the database" << endl;
    cout << "2. Display the contents of the database" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> choice;

    switch (choice) {
    case 1:
        addRow();
        break;
    case 2:
    {
        string readBuffer = "";
        displayDatabase(readBuffer);
        //std::vector<std::string> codenames;

        decodeJSON(readBuffer, codenames);

        std::cout << "Extracted codenames:" << std::endl;
        for (const auto& codename : codenames) {
            std::cout << codename << std::endl;
        }
        break;
    }
    default:
        cerr << "Invalid choice" << endl;
        break;
    }

    return 0;
}

//g++ -Wall *.cpp -o supa.exe  -lcurl

//g++ -Wall *.cpp -o test.exe -lsfml-graphics -lsfml-window -lsfml-system -lcurl
