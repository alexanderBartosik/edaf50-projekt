#include "DBmem.h"
#include "NewsGroup.h"
#include "Article.h"

#include <unordered_map>
#include <string>
#include <list>
#include <iostream>

using namespace std;

DBmem :: DBmem() {
    // Constructor
}

DBmem :: ~DBmem() {
    // Destructor
}

list<NewsGroup> DBmem :: listNewsGroup() const {
    // List all news groups
    list<NewsGroup> newsGroupList;
    for (auto it = newsGroups.begin(); it != newsGroups.end(); ++it) {
        newsGroupList.push_back(it->second);
    }
    return newsGroupList;
}

void DBmem :: addNewsGroup(string newsGroupName) {
    // Add a news group
    for(auto i : newsGroups){
        if(i.second.getName() == newsGroupName){
            cout << "News group already exists" << endl;
            return;
        }
    }
    NewsGroup newsGroup(newsGroupName, newsGroupId);
    newsGroups.insert({newsGroupId, newsGroup});
    newsGroupId++;
}

string DBmem :: getNewsGroup(string newsGroupName) const {
    // Get a news group
    for(auto i : newsGroups){
        if(i.second.getName() == newsGroupName){
            return i.second.getName();
        }
    }
    return "News group not found";
}

void DBmem :: removeNewsGroup(int newsGroupId) {
    // Remove a news group
    for(auto i : newsGroups){
        if(i.second.getId() == newsGroupId){
            newsGroups.erase(i.first);
            return;
        }
    }
    cout << "News group not found" << endl;
}

list<Article> DBmem :: listArticles(int newsGroupId) const {
    // List all articles in a news group
    list<Article> articleList;
    for(auto i : newsGroups){
        if(i.second.getId() == newsGroupId){
            for(auto j : i.second.getArticles()){
                articleList.push_back(j);
            }
        }
    }
    return articleList;
}

void DBmem :: addArticle(string title, string author, string text, int newsGroupId) {
    // Add an article to a news group
    //newsGroups[newGroupId].addArticle(title, author, text);
    newsGroups.at(newsGroupId).addArticle(title, author, text);
}

Article DBmem :: getArticle(int articleId, int newGroupId) const {
    // Get an article
    //return newsGroups[newGroupId].getArticle(articleId);
    return newsGroups.at(newGroupId).getArticle(articleId);
}

void DBmem :: removeArticle(int articleId, int newsGroupId) {
    // Remove an article
    //newsGroups[newsGroupId].removeArticle(articleId);
    newsGroups.at(newsGroupId).removeArticle(articleId);
}




