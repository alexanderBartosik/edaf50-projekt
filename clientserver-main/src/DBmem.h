#ifndef MEMORYDB_H
#define MEMORYDB_H

#include "DBinterface.h"
#include <unordered_map>
#include <string>

class DBmem : public DBinterface {
public:
    virtual std::list<NewsGroup> listNewsGroup() const = 0;

    virtual void addNewsGroup(string newsGroupName) = 0;

    virtual std::string getNewsGroup(string newsGroupName) const = 0;

    virtual void removeNewsGroup(int newsGroupId) = 0;

    virtual std::list<Article> listArticles(int newsGroupId) const = 0;

    virtual void addArticle(string title, string author, string text, int newGroupId) = 0;

    virtual pair<int,Article> getArticle(int articleId, int newGroupId) const = 0;

    virtual void removeArticle(int articleId, int newsGroupId) = 0;

    virtual ~DBmem() {}

private:
    int newsGroupId = 0;
    std::map<int, NewsGroup> newsGroups;
};

#endif