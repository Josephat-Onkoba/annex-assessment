#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

struct Article {
    string name;
    int num_comments;
};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {

    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

string fetchAPI(string url) {

    CURL* curl;
    CURLcode res;
    string response;
    curl = curl_easy_init();

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

bool compareArticles(const Article& a, const Article& b) {

    if (a.num_comments != b.num_comments) {
        return a.num_comments > b.num_comments;
    }
    return a.name > b.name;
}

vector<string> topArticles(int limit) {

    vector<Article> all_articles;

    int total_pages = 1;
    int current_page = 1;

    while (current_page <= total_pages) {

        string url =
            "https://jsonmock.hackerrank.com/api/articles?page="
            + to_string(current_page);

        string response = fetchAPI(url);
        json j = json::parse(response);
        total_pages = j["total_pages"];

        for (auto& item : j["data"]) {

            string final_name = "";

            if (!item["title"].is_null()) {
                final_name = item["title"];
            }
            else if (!item["story_title"].is_null()) {
                final_name = item["story_title"];
            }
            else {
                continue;
            }

            int comments = 0;

            if (!item["num_comments"].is_null()) {
                comments = item["num_comments"];
            }

            Article article;

            article.name = final_name;
            article.num_comments = comments;
            all_articles.push_back(article);
        }

        current_page++;
    }

    sort(all_articles.begin(), all_articles.end(), compareArticles);

    vector<string> result;

    int items_to_get = min(limit, (int)all_articles.size());

    for (int i = 0; i < items_to_get; i++) {
        result.push_back(all_articles[i].name);
    }

    return result;
}