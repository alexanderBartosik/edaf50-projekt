#include "DBmem.h"
#include "NewsGroup.h"
#include "Article.h"

#include <unordered_map>
#include <string>
#include <list>
#include <iostream>

using namespace std;

list<NewsGroup> DBmem :: listNewsGroup() const {
    // List all news groups
    list<NewsGroup> newsGroupList;
    for(auto i : newsGroups){
        newsGroupList.push_back(i.second);
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

bool DBmem :: newsGroupExists(int newsGroupId) const {
    auto it = newsGroups.find(newsGroupId);
    return it != newsGroups.end();
}

bool DBmem :: removeNewsGroup(int newsGroupId) {
    // Remove a news group
    for(auto i : newsGroups){
        if(i.second.getId() == newsGroupId){
            newsGroups.erase(i.first);
            return true;
        }
    }
    return false;
}

list<Article> DBmem :: listArticles(int ngid) const {
    // List all articles in a news group
    list<Article> articleList;
    cout << "----[DBmem::listArticles] start----" << endl;
    cout << "ng id to find: " << ngid << endl;
    for(auto i : newsGroups){
        cout << "checking ng: " << i.second.getId() << " " << i.second.getName() << endl;
        if(i.second.getId() == ngid){
            cout << "ng found" << endl;
            cout << "ng artlist size " << i.second.getArticles().size() << endl; 
            for(auto j : i.second.getArticles()){
                cout << "appending article: " << j.getId() << " " << j.getTitle() << endl;
                articleList.push_back(j);
            }
        }
    }
    cout << "----[DBmem::listArticles] end----" << endl;
    return articleList;
}

bool DBmem :: addArticle(string title, string author, string text, int ngid) {
    // Add an article to a news group

    cout << "----[DBmem::addArticle] start----" << endl;

    //Checks if the news group DOES exist
    if(newsGroups.find(ngid) == newsGroups.end()){
        return false;
    }
    newsGroups.at(ngid).addArticle(title, author, text);
    cout << "name of Group to add article to: " << newsGroups.at(ngid).getName() << endl;
    cout << "Articles in Group after addArticles is called: " << endl;
    for(Article art : newsGroups.at(ngid).getArticles()){
        cout << "Article Id: " << art.getId() << endl;
    }
    cout << "----[DBmem::addArticle] end----" << endl;
    return true;

}

Article DBmem :: getArticle(int articleId, int ngid) const {
    // Get an article
    //return newsGroups[newGroupId].getArticle(articleId);

    //Checks if newsgroup does not exist return an article with id -1
    if(newsGroups.find(ngid) == newsGroups.end()){
        cout << "After calling getArticle if no group exist. " <<  endl;
        return Article("","","", -1);
    }
    //Checks if article does not exist return an article with id -2
    else if(newsGroups.at(ngid).getArticle(articleId).getId() == -2){
        return Article("","","", -2);
    }
    return newsGroups.at(ngid).getArticle(articleId);
}

int DBmem :: removeArticle(int articleId, int ngid) {
    // Remove an article
    
    //Check if the newsgroup doesnt exists
    if(newsGroups.find(ngid) == newsGroups.end()){
        return -1;
    }
    //Check if the article exists
    else if(newsGroups.at(ngid).getArticle(articleId).getId() == -2){
        return -2;
    }
    else{
        newsGroups.at(ngid).removeArticle(articleId);
        return 0;
    }
    
}




