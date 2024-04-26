#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
    public:
        Article(std::string title, std::string author, std::string text, int id);
        std::string getTitle() const;
        std::string getAuthor() const;
        std::string getText() const;
        int getId() const;
    private:
        std::string title;
        std::string author;
        std::string text;
        int id;
};

#endif