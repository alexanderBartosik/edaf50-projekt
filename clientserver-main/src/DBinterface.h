#ifndef MEMORYDB_H
#define MEMORYDB_H

#include <string>
#include <unordered_map>
using namespace std;

class DBinterface {
public:
    virtual void add(const std::string& key, const std::string& value) = 0;
    virtual std::string get(const std::string& key) const = 0;
    virtual void remove(const std::string& key) = 0;
    virtual ~DBinterface() {}
};

#endif