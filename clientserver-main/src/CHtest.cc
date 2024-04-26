#include "CommandHandler.h"
#include "../include/connection.h"
#include "../include/server.h"

#include <iostream>

using std::cout;
using std::endl;


int main() {
    auto server = Server(7777);
    shared_ptr<Connection> conn = std::make_shared<Connection>();
    server.registerConnection(conn);

    Connection connection = Connection("localhost", 7777);
    CommandHandler ch = CommandHandler(connection);

    ch.send_int(5);
    int answer = ch.receive_int(); 
    cout << "msg sent and received: " << answer << endl;
}