#include "DB.h"
#include "NewsGroup.h"
#include "Article.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <regex>


namespace fs = std::__fs::filesystem;
using namespace std;


DB::DB() {
    if (!fs::exists(dbPath)) { //om dbPath inte finns, skapa dbPath/ngList.txt
        fs::create_directory(dbPath);
        ofstream(dbPath / "ngList.txt");
    } else if (fs::exists(dbPath / "ngList.txt")) { //Om allt finns, ta in
        string ngid, artid;

        for (const auto & entry : fs::directory_iterator(dbPath)){
            if (entry.is_directory()){
                ngid = entry.path().stem().string();

                for (const auto& file : fs::directory_iterator(entry.path())) {
                    artid = (file.path().stem().string());
                    artID.emplace(stoi(ngid), stoi(artid));
                }
                newsGroupId = stoi(ngid) + 1;
            }
        }
    }else{//om dbPath finns men inte ngList.txt
        ofstream(dbPath / "ngList.txt");
    }
}

DB::~DB(){}

list<NewsGroup> DB :: listNewsGroup() const {
    list<NewsGroup> list;
    ifstream inFile(dbPath / "ngList.txt");
    string ngid, name;
    while (inFile >> ngid >> ws && getline(inFile, name)) {
        NewsGroup ng(name, stoi(ngid));
        list.push_back(ng);
    }
    return list;
}

bool DB :: newsGroupExists(int newsGroupId) const {
    list<NewsGroup> ngList = listNewsGroup();
    for(auto ng : ngList){
        if(ng.getId() == newsGroupId){
            return true;
        }
    }
    return false;
}

bool DB :: addNewsGroup(string newsGroupName) {
    list<NewsGroup> groups = listNewsGroup();
    for (const auto& ng : groups) {
        if (ng.getName() == newsGroupName) {
            return false;
        }
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

list<Article> DB :: listArticles(int newsGroupId) const {
    list<Article> articles;
    fs::path groupPath = dbPath / to_string(newsGroupId);
    if (!fs::exists(groupPath)){
        return articles;  // Return empty list if group does not exist
    }

    for (auto& p : fs::directory_iterator(groupPath)) {
        if (p.is_regular_file()) {
            ifstream inFile(p.path());
            string title, author, text, line;
            getline(inFile, title);
            getline(inFile, author);
            while (getline(inFile, line)) {
                text += line + "\n";
            }
            Article article(title, author, text, stoi(p.path().filename().string()));
            articles.push_back(article);
        }
    }
    articles.sort([](const Article& a, const Article& b) {
            return a.getId() < b.getId();
        });
    
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

int DB :: removeArticle(int articleId, int newsGroupId) {
    fs::path articlePath = dbPath / to_string(newsGroupId) / to_string(articleId);

    if (!fs::exists(dbPath / to_string(newsGroupId))) { //if the newsgroup doesn´t exists, return -2
        return -2;
    }   
    else if (!fs::exists(articlePath)) { // if the article doesn´t exists, return -1
        return -1;
    }
    else{ // if the article exists, remove it and return 0
        fs::remove(articlePath);
        return 0;
    }
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


