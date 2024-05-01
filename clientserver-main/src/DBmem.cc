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

bool DBmem :: addNewsGroup(string newsGroupName) {
    // Add a news group
    for(auto i : newsGroups){
        if(i.second.getName() == newsGroupName){
            cout << "News group already exists" << endl;
            return false;
        }
    }
    NewsGroup newsGroup(newsGroupName, newsGroupId);
    newsGroups.insert({newsGroupId, newsGroup});
    newsGroupId++;
    return true;
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

bool DBmem :: removeNewsGroup(int newsGroupId) {
    // Remove a news group
    for(auto i : newsGroups){
        if(i.second.getId() == newsGroupId){
            newsGroups.erase(i.first);
            return true;
        }
    }
    cout << "News group not found" << endl;
    return false;
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

bool DBmem :: addArticle(string title, string author, string text, int newsGroupId) {
    // Add an article to a news group
    //newsGroups[newGroupId].addArticle(title, author, text);
    newsGroups.at(newsGroupId).addArticle(title, author, text);
    return true;
}

Article DBmem :: getArticle(int articleId, int newGroupId) const {
    // Get an article
    //return newsGroups[newGroupId].getArticle(articleId);
    return newsGroups.at(newGroupId).getArticle(articleId);
}

bool DBmem :: removeArticle(int articleId, int newsGroupId) {
    // Remove an article
    //newsGroups[newsGroupId].removeArticle(articleId);
    newsGroups.at(newsGroupId).removeArticle(articleId);
    return true;
}




