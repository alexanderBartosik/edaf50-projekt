#ifndef MEMORYDB_H
#define MEMORYDB_H

#include "DBinterface.h"
#include <unordered_map>
#include <string>

class DBmem : public DBinterface {
public:
    void add(const std::string& key, const std::string& value) override;
    std::string get(const std::string& key) const override;
    void remove(const std::string& key) override;
private:
    std::unordered_map<std::string, std::string> db;
};

#endif