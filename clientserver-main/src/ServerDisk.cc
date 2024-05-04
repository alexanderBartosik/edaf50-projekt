// DBmemServer.cpp
#include "ServerInterface.h"
#include "DB.h"

class ServerDisk : public AbstractServer {
public:
    DBinterface* startDB() override {
        return new DB();
    }
};

int main(int argc, char *argv[]) {
        ServerDisk server;
        server.runServer(argc, argv);
        return 0;
}