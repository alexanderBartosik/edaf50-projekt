
#ifndef SERVER_INTERFACE_H
#define SERVER_INTERFACE_H

#include "../include/server.h"
#include "../include/protocol.h"
#include "../include/connection.h"
#include "../include/connectionclosedexception.h"
#include "CommandHandler.h"
#include "DBinterface.h"
#include "DB.h"
#include "DBmem.h"
#include <iostream>
#include <memory>

class AbstractServer {
public:
    virtual ~AbstractServer() {}

    virtual DBinterface* startDB() = 0;
    void list_ng(CommandHandler cmdh, DBinterface* db);
    void list_articles(CommandHandler cmdh, DBinterface* db);
    void get_article(CommandHandler cmdh, DBinterface* db);
    void create_ng(CommandHandler cmdh, DBinterface* db);
    void write_article(CommandHandler cmdh, DBinterface* db);
    void delete_ng(CommandHandler cmdh, DBinterface* db);
    void delete_article(CommandHandler cmdh, DBinterface* db);
    Server init(int argc, char* argv[]);
    void runServer(int argc, char *argv[]);
};

#endif
