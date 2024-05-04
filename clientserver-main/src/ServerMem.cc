#include "ServerInterface.h"
#include "DBmem.h"

class ServerMem : public AbstractServer {
public:
    DBinterface* startDB() override {
        return new DBmem();
    }
};

int main(int argc, char *argv[]) {
    ServerMem server;
    server.runServer(argc, argv);
    return 0;
}
