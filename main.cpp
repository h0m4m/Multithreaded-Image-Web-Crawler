#include <bits/stdc++.h>
#include <unordered_map>
#include <curl/curl.h>
#include <stdio.h>
#include <regex>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

mutex m;

//download html in file
void get_page(const char* url, const char* file_name)
{
    CURL* easyhandle = curl_easy_init(); // initialize the curl Object

    curl_easy_setopt(easyhandle, CURLOPT_URL, url);
    FILE* file = fopen(file_name, "w"); // make object for file handling

    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, file); // write to file
    curl_easy_perform(easyhandle); // structure the html code
    curl_easy_cleanup(easyhandle); // erase the html from curl_object/buffer
    fclose(file);
}

// extract image sources from file
vector<string> extract_image_sources(string html_file_name)
{
    string html; // here we will store the html from file
    ifstream read; // ifstream object for reading the file
    read.open(html_file_name);
    while (!read.eof()) // reading the file character by character
    {
        if (read.eof())
            break; // stop if file is ended
        char ch;
        read.get(ch);
        html.push_back(ch); // concatenate the character reading from file to string
    }
    read.close();
    static const regex img_regex("<img[^>]+src\\s*=\\s*['\"]([^'\"]+)['\"][^>]*>"); // regex for extracting image src
    vector<string> image_sources; // it will store all the image sources read from file
    // below line will store all found image sources in vector
    copy(sregex_token_iterator(html.begin(), html.end(), img_regex, 1), sregex_token_iterator(), back_inserter(image_sources));
    return image_sources;
}

// validate the image sources
// validate the image sources
void cleanUp(vector<string>& all_image_sources, const string& domain)
{
    vector<string> final_image_sources; // it will store all the valid image sources
    for (const auto& source : all_image_sources) // cleaner
    {
        string cleaned_source = source; // pick one image source

        // Check if the source already contains the domain, if not, append the domain to it
        if (cleaned_source.find(domain) == string::npos) {
            cleaned_source = domain + cleaned_source;
        }

        // it will store cleaned source
        final_image_sources.push_back(cleaned_source);
    }
    all_image_sources = final_image_sources; // replace it with final image sources vector
}


unordered_map<string, bool> visited; // used while dfs to identify if the URL is already visited or not + Insertion TC = O(1)

// dfs crawler function
void dfs_crawler(const char* url, const char* file_path, int depth, int bot_id)
{
    if (depth == 4 || visited[url])
        return; // if depth=4 or url is already visited
    string s = url; // store url in string s
    visited[s] = 1; // mark current url as visited

    m.lock(); // using it for synchronization
    cout << "Bot_id: " << bot_id << "\tLink: " << url << endl; // print the url and thread id
    get_page(url, file_path); // get html of current url

    vector<string> allImageSources = extract_image_sources(file_path); // get all image sources

    // Extract domain from the URL
    string domain = url;
    size_t domainEnd = domain.find("/", 8); // Find the end of the domain part in the URL
    if (domainEnd != string::npos) {
        domain = domain.substr(0, domainEnd); // Extract the domain part
    }

    // Validate and clean all the image sources
    vector<string> finalImageSources;
    for (const auto& source : allImageSources) {
        string cleanedSource = source;
        // Check if the source already contains the domain, if not, append the domain to it
        if (cleanedSource.find(domain) == string::npos) {
            cleanedSource = domain + cleanedSource;
        }
        finalImageSources.push_back(cleanedSource);
    }

    allImageSources = finalImageSources; // Replace with the cleaned image sources
    m.unlock(); // unlock the mutex lock

    // here the dfs
    for (const auto& i : allImageSources)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // sleep for 10ms to get the
        const char* c = i.c_str();
        if (visited[i] != 1) // if url is not visited then call the function
            dfs_crawler(c, file_path, depth + 1, bot_id);
    }
}


int main()
{
    const char* filename = "1.txt"; // file where html will be stored
    vector<thread> threads; // create a vector to store threads
    vector<string> urls; // create a vector to store URLs

    string input_url;
    while (true) {
        cout << "Enter a URL to crawl (or type 'quit' to exit): ";
        cin >> input_url;
        if (input_url == "quit") {
            break;
        }
        urls.push_back(input_url); // store the URL in the vector
    }

    for (size_t i = 0; i < urls.size(); ++i) {
        threads.emplace_back(dfs_crawler, urls[i].c_str(), filename, 0, i + 1); // add thread to the vector
    }

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join(); // join all the threads
        }
    }

    return 0;
}

