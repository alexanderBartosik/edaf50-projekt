#include "NewsGroup.h"
#include <string>
#include <list>
#include <map>
#include "Article.h"

using namespace std;

NewsGroup :: NewsGroup(string name, int id) : name(name), id(id) {
    // Constructor
}

string NewsGroup :: getName() const {
    // Get name
    return name;
}

int NewsGroup :: getId() const {
    // Get id
    return id;
}

Article NewsGroup :: getArticle(int articleId) const {
    // Get article
    for (auto i : articles) {
        if (i.first == articleId) {
            return i.second;
            //return make_pair(i.first, i.second);
        }
    }
    return Article("", "", "", -2);
}

void NewsGroup :: addArticle(string title, string author, string text) {
    // Add article
    Article article(title, author, text, articleId);
    articles.emplace(articleId, article);
    articleId++;
}

void NewsGroup :: removeArticle(int articleId) {
    // Remove article
    for (auto i : articles) {
        if (i.second.getId() == articleId) {
            articles.erase(i.first);
            return;
        }

    }
}

list<Article> NewsGroup :: getArticles() const {
    // Get articles
    list<Article> articleList;
    for (auto i : articles) {
        articleList.push_back(i.second);
    }
    return articleList;
}


