#include "../include/server.h"
#include "../include/protocol.h"
#include "../include/connection.h"
#include "../include/connectionclosedexception.h"
#include "CommandHandler.h"

#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::cin;

/* (myclient.cc) Creates a client for the given args, if possible.
 * Otherwise exits with error code.
 */
Connection init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: myclient host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = std::stoi(argv[2]);
        } catch (std::exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        Connection conn(argv[1], port);
        if (!conn.isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}

void list_ng(CommandHandler cmdh) {

}

void list_articles(CommandHandler cmdh) {
    
}

void read_articles(CommandHandler cmdh) {
    
}

void read_articles(CommandHandler cmdh) {
    
}



int main(int argc, char* argv[])
{
        Connection conn = init(argc, argv);
        CommandHandler cmdh(conn);

        cout << "Welcome. Choose one of the following options to proceed." << endl;
        cout << "1: List news groups." << endl;
        cout << "2: List articles in news group." << endl;
        cout << "3: Read article." << endl;
        cout << "4: Create news group." << endl;
        cout << "5: Create article." << endl;
        cout << "6: Delete news group." << endl;
        cout << "7: Delete article." << endl;
        cout << "8: Exit." << endl;

        int nbr;
        while (true)
        {
            try
            {
                if (cin >> nbr)
                {

                    switch (nbr)
                    {

                        // TODO

                    default: // case 8;
                        cout << "exiting..." << endl;
                        return 0;
                    }
                }
                else
                {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // slackfråga
                    cout << "Invalid input, try again." << endl;
                }
            }
            catch (ConnectionClosedException &)
            {
                cout << "Connection error, exiting..." << endl;
                return 1;
            }
        }
}
