#ifndef DB_H
#define DB_H

#include "dBinterface.h"
#include "newsgroup.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class DB : public DBinterface {

public:
    
    DB() {}

    std::list<NewsGroup> listNewsGroup() const;

    bool addNewsGroup(string newsGroupName);

    std::string getNewsGroup(string newsGroupName) const;

    bool removeNewsGroup(int newsGroupId);

    std::list<Article> listArticles(int newsGroupId) const;

    bool addArticle(string title, string author, string text, int newGroupId);

    Article getArticle(int articleId, int newGroupId) const;

    bool removeArticle(int articleId, int newsGroupId);

    ~DB() {}

private:
    int newsGroupId = 0;
    //std::map<int, NewsGroup> newsGroups;
    fs::path dbPath = "database.db";
    unordered_map<int, int> artID;  // Mapping from newsgroup ID to next article ID

};

#endif