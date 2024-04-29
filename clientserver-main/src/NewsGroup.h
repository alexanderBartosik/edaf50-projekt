#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <list>
#include <map>
#include "Article.h"

class NewsGroup {
public:
    NewsGroup(std::string name, int id);

    std::string getName() const;

    int getId() const;

    //std::pair<int, Article> getArticle(int id) const;
    Article getArticle(int id) const;

    std::list<Article> getArticles() const;

    void addArticle(std::string title, std::string author, std::string text);

    void removeArticle(int articleId);

private:
    std::string name;
    int id;
    int articleId = 0;
    std::map<int, Article> articles;
};

#endif