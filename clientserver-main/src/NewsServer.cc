#include "../include/server.h"
#include "../include/protocol.h"
#include "../include/connection.h"
#include "../include/connectionclosedexception.h"
#include "CommandHandler.h"

#include "DBinterface.h" //?
#include "DB.h"
#include "DBmem.h"
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::endl;



void list_ng() {
} 

void get_ng(int ngID) {
}

void list_articles() {
}

void get_article(int ngId, int artID) {
}

Server init(int argc, char* argv[]) {
        if (argc != 2) {
                cerr << "Usage: myserver port-number" << endl;
                exit(1);
        }
        int port = -1;
        try {
                port = std::stoi(argv[1]);
        } catch (std::exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }
        cout << "Server started." << endl;
        return server;
}



int main(int argc, char *argv[])
{
    Server server = init(argc, argv);
    //TODO: make database

    while (true)
    {
        auto conn = server.waitForActivity();
        if (conn != nullptr)
        {
            CommandHandler ch(conn); //shared_ptr or not, might have to change!
            try
            {
                Protocol msg = ch.receive_command();

                switch(msg) {
                    //TODO: call methods depending on command
                }

            }
            catch (ConnectionClosedException &)
            {
                server.deregisterConnection(conn);
                cout << "Connection terminated" << endl;
            }
        }
        else
        {
            conn = std::make_shared<Connection>();
            server.registerConnection(conn);
            cout << "New client connects" << endl;
        }
    }

    return 0;
}