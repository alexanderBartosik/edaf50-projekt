#ifndef MEMORYDB_H
#define MEMORYDB_H

#include "DBinterface.h"
#include <unordered_map>
#include <string>

class DBmem : public DBinterface {
public:

    DBmem() {}

    std::list<NewsGroup> listNewsGroup() const = 0;

    bool addNewsGroup(string newsGroupName) = 0;

    std::string getNewsGroup(string newsGroupName) const = 0;

    bool removeNewsGroup(int newsGroupId) = 0;

    std::list<Article> listArticles(int newsGroupId) const = 0;

    bool addArticle(string title, string author, string text, int newGroupId) = 0;

    //virtual pair<int,Article> getArticle(int articleId, int newGroupId) const = 0;
    Article getArticle(int articleId, int newGroupId) const = 0;

    bool removeArticle(int articleId, int newsGroupId) = 0;

    ~DBmem() {}

private:
    int newsGroupId = 0;
    std::map<int, NewsGroup> newsGroups;
};

#endif