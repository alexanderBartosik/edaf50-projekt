#include "DB.h"
#include "NewsGroup.h"
#include "Article.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <regex>


namespace fs = std::filesystem;
using namespace std;


DB::DB() {
    if (!fs::exists(dbPath)) {
        fs::create_directory(dbPath);
        ofstream(dbPath / "ngList.txt");
    } else if (fs::exists(dbPath / "ngList.txt")) {
        ifstream inFile(dbPath / "ngList.txt");
        string ngid, artid, ignore;
        while (inFile >> ngid >> artid >> ignore) {
            getline(inFile, ignore);  // To consume the rest of the line
            artID.emplace(stoi(ngid), stoi(artid));
            newsGroupId = stoi(ngid) + 1;
        }
    }
}

DB::~DB(){}

list<NewsGroup> DB :: listNewsGroup() const {
    list<NewsGroup> list;
    ifstream inFile(dbPath / "ngList.txt");
    string ngid, name;
    while (inFile >> ngid >> ws && getline(inFile, name)) {
        list.emplace_back(name, stoi(ngid));
    }
    return list;
}

bool DB :: addNewsGroup(string newsGroupName) {
    list<NewsGroup> groups = listNewsGroup();
    if (find_if(groups.begin(), groups.end(), [&](const NewsGroup& ng) { //Ändra kanske denna 
            return ng.getName() == newsGroupName;
        }) != groups.end()) {
        return false;
    }

    fs::create_directory(dbPath / to_string(newsGroupId));
    ofstream outFile(dbPath / "ngList.txt", ios::app);
    outFile << newsGroupId << " " << newsGroupName << "\n";
    artID[newsGroupId] = 1;
    newsGroupId++;
    return true;
}

bool DB :: removeNewsGroup(int newsGroupId) {
    if (!fs::exists(dbPath / to_string(newsGroupId))) {
        return false;
    }
    fs::remove_all(dbPath / to_string(newsGroupId));
    artID.erase(newsGroupId);

    // Update ngList.txt
    list<NewsGroup> groups = listNewsGroup();
    ofstream outFile(dbPath / "ngList.txt");
    for (const auto& ng : groups) {
        if (ng.getId() != newsGroupId) {
            outFile << ng.getId() << " " << ng.getName() << "\n";
        }
    }
    return true;
}

list<Article> DB ::  listArticles(int newsGroupId) const {
    list<Article> articles;
    fs::path groupPath = dbPath / to_string(newsGroupId);
    if (!fs::exists(groupPath)) return articles;  // Return empty list if group does not exist

    for (auto& p : fs::directory_iterator(groupPath)) {
        if (p.is_regular_file()) {
            ifstream inFile(p.path());
            string title, author, text, line;
            getline(inFile, title);
            getline(inFile, author);
            while (getline(inFile, line)) {
                text += line + "\n";
            }
            articles.emplace_back(title, author, text, stoi(p.path().filename().string()));
        }
    }
    return articles;
}

bool DB :: addArticle(string title, string author, string text, int newsGroupId) {
    fs::path groupPath = dbPath / to_string(newsGroupId);
    if (!fs::exists(groupPath)) return false;  // Newsgroup must exist

    int articleId = artID[newsGroupId];  // Get next article ID
    ofstream outFile(groupPath / to_string(articleId));
    if (outFile.is_open()) {
        outFile << title << '\n' << author << '\n' << text;
        outFile.close();
        artID[newsGroupId]++;  // Increment the article ID for the next article
        return true;
    }
    return false;
}

Article DB :: getArticle(int articleId, int newsGroupId) const {
    fs::path articlePath = dbPath / to_string(newsGroupId) / to_string(articleId);
    if (!fs::exists(articlePath)) {
        //throw exception("Article not found");
        return Article("Article not found", "", "", -1);
    }

    ifstream inFile(articlePath);
    string title, author, text, line;
    getline(inFile, title);
    getline(inFile, author);
    while (getline(inFile, line)) {
        text += line + (inFile.eof() ? "" : "\n");
    }
    return Article(title, author, text, articleId);
}

bool DB :: removeArticle(int articleId, int newsGroupId) {
    fs::path articlePath = dbPath / to_string(newsGroupId) / to_string(articleId);
    if (fs::exists(articlePath)) {
        fs::remove(articlePath);
        return true;
    }
    return false;
}

string DB :: getNewsGroup(string newsGroupName) const {
    ifstream inFile(dbPath / "ngList.txt");
    string ngid, name;
    while (inFile >> ngid >> ws && getline(inFile, name)) {
        if (name == newsGroupName) {
            return name;
        }
    }
    return "News group not found";
}


