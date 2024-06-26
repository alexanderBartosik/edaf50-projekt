#ifndef DBINTERFACE_H
#define DBINTERFACE_H

#include <string>
#include <unordered_map>
#include <list>
#include "NewsGroup.h"
using namespace std;

class DBinterface {
public:
    virtual std::list<NewsGroup> listNewsGroup() const = 0;

    virtual bool addNewsGroup(string newsGroupName) = 0;

    virtual std::string getNewsGroup(string newsGroupName) const = 0;

    virtual bool newsGroupExists(int newsGroupId) const = 0; 

    virtual bool removeNewsGroup(int newsGroupId) = 0;

    virtual std::list<Article> listArticles(int newsGroupId) const = 0;

    virtual bool addArticle(string title, string author, string text, int newGroupId) = 0;

    //returns Article with id -1 if ng not found, -2 if article not found.
    virtual Article getArticle(int articleId, int newGroupId) const = 0; 

    //-1: ng not found, -2 article not found, 0 success.
    virtual int removeArticle(int articleId, int newsGroupId) = 0; 

    virtual ~DBinterface() {}
};

#endif