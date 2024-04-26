#include "Article.h"
#include <string>
#include <list>
#include <map>

using namespace std;

Article :: Article(string title, string author, string text, int id) : title(title), author(author), text(text), id(id) {
    // Constructor
}

string Article :: getTitle() const {
    // Get title
    return title;
}

string Article :: getAuthor() const {
    // Get author
    return author;
}

string Article :: getText() const {
    // Get text
    return text;
}

int Article :: getId() const {
    // Get id
    return id;
}




