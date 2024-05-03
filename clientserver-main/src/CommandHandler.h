#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "../include/protocol.h"
#include "../include/connection.h"
#include <memory> 
#include <string>

using std::shared_ptr;


/*
MESSAGES:
commandbyte parameters endbyte
- commandbyte:  e.g. COM_LIST_NG (1 byte)
- parameters:   starts with either PAR_NUM or PAR_STRING (1 byte). Then either N (4 bytes, significant byte first, MSB) 
                or N (same, representing nr of chars) and char1 ... charN (N bytes), respectively. 
- endbyte:      COM_END or ANS_END (1 byte)

example: 
ANS_LIST_NG 2 13 comp.lang.java 15 comp.lang.c++ ANS_END (2: nr of NGs, 13 and 15: NG IDs)
where "2", "13", "comp.lang.java", etc are coded as PAR_NUM 2, PAR_NUM 13, PAR_STRING 14 c o m p . (...)"

*/
class CommandHandler {
    public:
    CommandHandler(shared_ptr<Connection>& conn);
    CommandHandler(Connection& conn);

    void send_int(int val); //sends PAR_NUM val
    void send_string(std::string str); //sends PAR_STRING N str (conn.write char by char)
    void send_command(Protocol code); 

    int receive_int();
    std::string receive_string();
    Protocol receive_command();

    void assert_ended(); //throwar ConnectionClosedException om byten != COM_END


    private:
    shared_ptr<Connection> conn;
    int read_int();
};

#endif