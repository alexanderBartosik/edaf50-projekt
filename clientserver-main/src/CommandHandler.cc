#include "CommandHandler.h"
#include "../include/connectionclosedexception.h"
#include <iostream>

using std::cout;
using std::endl;

//antar just nu att denna används bara!
CommandHandler::CommandHandler(shared_ptr<Connection>& c): conn(c) {}

//myclient.cc tar Connection&, idk?
CommandHandler::CommandHandler(Connection& c): conn(&c) {}


void CommandHandler::send_int(int val) {
    send_command(Protocol::PAR_NUM); 

    conn->write((val >> 24) & 0xFF);
    conn->write((val >> 16) & 0xFF);
    conn->write((val >> 8) & 0xFF);
    conn->write(val & 0xFF);
}

void CommandHandler::send_string(std::string val){
    send_command(Protocol::PAR_STRING);
    int length = static_cast<int>(val.length());

    //bryt ut ksk
    conn->write((length >> 24) & 0xFF); 
    conn->write((length >> 16) & 0xFF);
    conn->write((length >> 8) & 0xFF);
    conn->write(length & 0xFF);

    for (char c : val) {
        conn->write(c);
    }

}

void CommandHandler::send_command(Protocol code) {
    conn->write(static_cast<unsigned char>(code));
}


int CommandHandler::receive_int() {
    if (receive_command() == Protocol::PAR_NUM) {
        return read_int();
    } else {
        throw ConnectionClosedException();
    }
}

int CommandHandler::read_int() {
    unsigned char byte1 = conn->read();
    unsigned char byte2 = conn->read();
    unsigned char byte3 = conn->read();
    unsigned char byte4 = conn->read();
    return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

std::string CommandHandler::receive_string() {
    if (receive_command() == Protocol::PAR_STRING) {
        std::string s;
        char ch;

        int slen = read_int();

        for (int i = 0; i < slen; i++) {
            ch = conn->read();
            s+= ch;
        }
        return s;
    } else {
        throw_protocol_error();
    }
        
}
    
Protocol CommandHandler::receive_command() {
    return static_cast<Protocol>(conn->read());
}

void CommandHandler::com_ended() {
    if (receive_command() != Protocol::COM_END) throw_protocol_error();
}

void CommandHandler::ans_ended() {
    if (receive_command() != Protocol::ANS_END) throw_protocol_error();
}


void CommandHandler::throw_protocol_error() {
    cout << "ERROR: protocol violated" << endl;
    throw ConnectionClosedException();
}