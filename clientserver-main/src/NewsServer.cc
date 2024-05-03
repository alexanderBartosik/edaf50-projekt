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



void list_ng(CommandHandler cmdh, std::unique_ptr<DBinterface> db) {//Alex
    cmdh.assert_ended();

    std::list<NewsGroup> nglist = db->listNewsGroup();
    int ngsize = nglist.size();

    cmdh.send_command(Protocol::ANS_LIST_NG);
    cmdh.send_int(ngsize);

    for (NewsGroup ng : nglist) {        
        cmdh.send_int(ng.getId());
        cmdh.send_string(ng.getName());
    }

    cmdh.send_command(Protocol::ANS_END);
} 

void list_articles(CommandHandler cmdh, std::unique_ptr<DBinterface> db) { 
    cmdh.send_command(Protocol::ANS_LIST_ART);

    int newsGroupID = cmdh.receive_int();

    std::list<Article> articles = db->listArticles(newsGroupID);

    cmdh.send_int(articles.size());

    //If the newsgroup id does not exist
    if (articles.size() == 0) { //TODO: check if this is correct
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    else {
        cmdh.send_command(Protocol::ANS_ACK);
        cmdh.send_int(articles.size());
        for (Article a : articles) {
            cmdh.send_int(a.getId());
            cmdh.send_string(a.getTitle());
        }
    }
    cmdh.send_command(Protocol::ANS_END);
}

void get_article(CommandHandler cmdh, std::unique_ptr<DBinterface> db) { 
    cmdh.send_command(Protocol::ANS_GET_ART);

    int newsGroupID = cmdh.receive_int();
    int articleID = cmdh.receive_int();

    Article article = db->getArticle(articleID, newsGroupID);

    if (article.getId() == -1) {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_ART_DOES_NOT_EXIST);
    }
    else {
        cmdh.send_command(Protocol::ANS_ACK);
        cmdh.send_string(article.getTitle());
        cmdh.send_string(article.getAuthor());
        cmdh.send_string(article.getText());
    }
    cmdh.send_command(Protocol::ANS_END);
}



void create_ng(CommandHandler cmdh, std::unique_ptr<DBinterface> db) { //ALEX
    string ngName = cmdh.receive_string();

    cmdh.assert_ended();
 
    bool created = db->addNewsGroup(ngName);

    cmdh.send_command(Protocol::ANS_CREATE_NG);

    if (created) {
        cmdh.send_command(Protocol::ANS_ACK);
    } else {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    cmdh.send_command(Protocol::ANS_END);
}

void write_article(CommandHandler cmdh, std::unique_ptr<DBinterface> db) { //Alex
    int ngId = cmdh.receive_int();
    string title = cmdh.receive_string();
    string author = cmdh.receive_string();
    string text = cmdh.receive_string();

    cmdh.assert_ended();

    bool created = db->addArticle(title, author, text, ngId);

    if (created) {
        cmdh.send_command(Protocol::ANS_ACK);
    } else {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    cmdh.send_command(Protocol::ANS_END);
}

void delete_ng(CommandHandler cmdh, std::unique_ptr<DBinterface> db) { //Alex 
    int ngId = cmdh.receive_int();

    cmdh.assert_ended();

    bool removed = db->removeNewsGroup(ngId);
    
    if (removed) {
        cmdh.send_command(Protocol::ANS_ACK);
    } else {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_NG_DOES_NOT_EXIST);
    }
    cmdh.send_command(Protocol::ANS_END);
}

void delete_article(CommandHandler cmdh, std::unique_ptr<DBinterface> db) {
    int ngId = cmdh.receive_int();
    int artId = cmdh.receive_int();

    cmdh.assert_ended();

    int removed = db->removeArticle(artId, ngId);

    if (removed == 0) {
        cmdh.send_command(Protocol::ANS_ACK);
    } else if (removed == -1) {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_NG_DOES_NOT_EXIST);
    } else if (removed == -2) {
        cmdh.send_command(Protocol::ANS_NAK);
        cmdh.send_command(Protocol::ERR_ART_DOES_NOT_EXIST); 
    }

    cmdh.send_command(Protocol::ANS_END);
}


//(myserver.cc)
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

std::unique_ptr<DBinterface> startDB() { //ptr?
    return std::make_unique<DBmem>();
}



int main(int argc, char *argv[])
{
    Server server = init(argc, argv);
    std::unique_ptr<DBinterface> db = startDB();

    while (true)
    {
        auto conn = server.waitForActivity();
        if (conn != nullptr)
        {
            CommandHandler cmdh(conn); //shared_ptr or not, might have to change!
            try
            {
                Protocol msg = cmdh.receive_command();

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