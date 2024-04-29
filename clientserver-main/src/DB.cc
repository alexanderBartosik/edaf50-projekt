#include "DB.h"
#include "NewsGroup.h"
#include "Article.h"

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <list>
#include <iostream>
#include <fstream>


using namespace std;

DB :: DB() {
    // Constructor
    //The disk database should be implemented using the standard library <filesystem>


}

list<NewsGroup> DB :: listNewsGroup() const {
    ifstream file("newsgroups.txt");
    list<NewsGroup> newsGroupList;
    string line;
    while(getline(file, line)){
        string name = line.substr(0, line.find(" "));
        int id = stoi(line.substr(line.find(" ")+1));
        NewsGroup newsGroup(name, id);
        newsGroupList.push_back(newsGroup);
    }
    return newsGroupList;
}