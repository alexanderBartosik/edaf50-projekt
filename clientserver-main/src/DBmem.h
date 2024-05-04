#ifndef MEMORYDB_H
#define MEMORYDB_H

#include "DBinterface.h"
#include <unordered_map>
#include <string>

class DBmem : public DBinterface {
public:

    DBmem() {}

    std::list<NewsGroup> listNewsGroup() const;

    bool addNewsGroup(string newsGroupName);

    std::string getNewsGroup(string newsGroupName) const;

    bool newsGroupExists(int newsGroupId) const;

    bool removeNewsGroup(int newsGroupId);

    std::list<Article> listArticles(int newsGroupId) const;

    bool addArticle(string title, string author, string text, int newGroupId);

    //virtual pair<int,Article> getArticle(int articleId, int newGroupId) const = 0;
    Article getArticle(int articleId, int newGroupId) const;

    int removeArticle(int articleId, int newsGroupId);

    ~DBmem() {}

private:
    int newsGroupId = 0;
    std::map<int, NewsGroup> newsGroups;
};

#endif