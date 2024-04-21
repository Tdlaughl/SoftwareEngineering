#ifndef supa_h
#define supa_h

#include <iostream>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <vector>

using namespace std;

class Database
{
    public:
        Database();
        const string SUPABASE_URL = "https://xjvowcvengujotdcwbjm.supabase.co";
        const string SUPABASE_API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Inhqdm93Y3Zlbmd1am90ZGN3YmptIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MDc3Nzg4NzksImV4cCI6MjAyMzM1NDg3OX0.eEVskzeaC4ZKqL_GbGkJZzC2WNb01VrKnsKtKNZVm94";
        
        //moved from a declaration inside supa(), for easy access in PlayerEntry
        std::vector<std::string> codenames;
        std::vector<int> ids;
        
        //Methods
        void addRow(int idNumber, string codeName);
        static size_t writeCallback(void* ptr, size_t size, size_t nmemb, string* data);
        void displayDatabase(string& readBuffer);
        void decodeJSON(const std::string& jsonString, std::vector<std::string>& codenames); 
        int supa();  //the old main method
};
#endif
