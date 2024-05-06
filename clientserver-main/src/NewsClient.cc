#include "server.h"
#include "protocol.h"
#include "connection.h"
#include "connectionclosedexception.h"
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
std::shared_ptr<Connection> init(int argc, char* argv[])
{
        if (argc != 3) {
                cerr << "Usage: host-name port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = std::stoi(argv[2]);
        } catch (std::exception& e) {
                cerr << "Wrong port number. " << e.what() << endl;
                exit(2);
        }

        std::shared_ptr<Connection> conn = std::make_shared<Connection>(argv[1], port);

        if (!conn->isConnected()) {
                cerr << "Connection attempt failed" << endl;
                exit(3);
        }

        return conn;
}

bool acked(CommandHandler cmdh) {
    Protocol msg = cmdh.receive_command();

    if (msg == Protocol::ANS_ACK) {
        return true;
    } else if (msg != Protocol::ANS_NAK) {
        cmdh.throw_protocol_error();
    }
    else {//ANS_NAK
        switch (cmdh.receive_command()) {
            case Protocol::ERR_ART_DOES_NOT_EXIST:
                cout << "ERROR: article does not exist. Write an article with '5'" << endl;
                break;
            case Protocol::ERR_NG_ALREADY_EXISTS:
                cout << "ERROR: news group already exists. List news groups with '1'" << endl;
                break;
            case Protocol::ERR_NG_DOES_NOT_EXIST:
                cout << "ERROR: news group doesn't exist. Create a news group with '4'" << endl;
                break;
            default:
                cout << "ERROR: unknown protocol error" << endl;
                break;
        }
    }
    return false;
}

void list_ng(CommandHandler cmdh) {
    cmdh.send_command(Protocol::COM_LIST_NG);
    cmdh.send_command(Protocol::COM_END);

    Protocol cmd1 = cmdh.receive_command();
    //cout << static_cast<unsigned char>(cmd1) << endl;

    if (cmd1 == Protocol::ANS_LIST_NG) {

        int groupCount = cmdh.receive_int();

        if (groupCount > 0) {
            cout << "group ID | group name:" << endl;
            for (int i = 0; i < groupCount; i++) {
                int groupId = cmdh.receive_int();

                string groupName = cmdh.receive_string();
                cout << groupId << " | " << groupName << endl;
            }
        } else {
            cout << "There are currently no news groups" << endl;
        } 
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void create_ng(CommandHandler cmdh) {
    cout << "Enter the name of your news group" << endl;
    cin.ignore(); //clear '4'
    string ngName;
    getline(std::cin, ngName);

    cmdh.send_command(Protocol::COM_CREATE_NG);
    cmdh.send_string(ngName);
    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_CREATE_NG) {
        if (acked(cmdh)) {
            cout << "News group " << ngName << " created." << endl;
        }
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void delete_ng(CommandHandler cmdh) {
    cout << "Enter ID of the news group to delete" << endl;
    cin.ignore();
    string ngIdString;
    getline(std::cin, ngIdString);
    int ngId;

    try {
        ngId = std::stoi(ngIdString);
    } catch (std::invalid_argument) {
        cout << "ERROR: invalid input. Retry command/try another command." << endl;
        return;
    }

    cmdh.send_command(Protocol::COM_DELETE_NG);
    cmdh.send_int(ngId);
    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_DELETE_NG) {
        if (acked(cmdh)) {
            cout << "News group with ID " << ngId << " deleted." << endl;
        } 
    } else {
        cmdh.throw_protocol_error();
        }

    cmdh.ans_ended();
}

void list_articles(CommandHandler cmdh) {
    cout << "Enter ID of the news group you wish to list the articles of" << endl;
    cin.ignore();
    string ngIdString;
    getline(cin, ngIdString);
    int ngId;

    try {
        ngId = std::stoi(ngIdString);
    } catch (std::invalid_argument) {
        cout << "ERROR: invalid input. Retry command/try another command." << endl;
        return;
    }

    cmdh.send_command(Protocol::COM_LIST_ART);
    cmdh.send_int(ngId);
    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_LIST_ART) {
        if (acked(cmdh)) {
            int articleCount = cmdh.receive_int();
            if (articleCount > 0) {
                cout << "article ID | article title:" << endl;
                for (int i = 0; i < articleCount; i++) {
                    int articleId = cmdh.receive_int();
                    string articleTitle = cmdh.receive_string();
                    cout << articleId << " | " << articleTitle << endl;
                }
            } else {
                cout << "This news group currently has no articles" << endl;
            }
        }
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void read_article(CommandHandler cmdh) {    
    cout << "Enter id of news group you'd like to read an article of" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    string ngIdString;
    getline(cin, ngIdString);
    int ngId;

    try {
        ngId = std::stoi(ngIdString);
    } catch (std::invalid_argument) {
        cout << "ERROR: invalid input. Retry command/try another command." << endl;
        return;
    }

    cout << "Enter id of article you'd like to read" << endl;
    string artIdString;
    getline(cin, artIdString);
    int artId;

    try {
        artId = std::stoi(artIdString);
    } catch (std::invalid_argument) {
        cout << "ERROR: invalid input. Retry command/try another command." << endl;
        return;
    }

    cmdh.send_command(Protocol::COM_GET_ART);
    cmdh.send_int(ngId);
    cmdh.send_int(artId);
    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_GET_ART) {
        if (acked(cmdh)) {
            string title = cmdh.receive_string();
            string author = cmdh.receive_string();
            string text = cmdh.receive_string();
            cout << "'"<< title <<"'"<< " by " << author << endl;
            cout << text << endl;
        }
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void write_article(CommandHandler cmdh) {
    cmdh.send_command(Protocol::COM_CREATE_ART);

    cout << "Enter ID of the news group you wish to write an article to" << endl;
    cin.ignore();
    string ngIdString;
    int ngId;
    getline(cin, ngIdString);
    try {
        ngId = std::stoi(ngIdString);
    } catch (std::invalid_argument) {
        cout << "ERROR: invalid input. Retry command/try another command." << endl;
        return;
    }
    cmdh.send_int(ngId);

    cout << "Enter title of the article" << endl;
    string title;
    getline(cin, title);
    cmdh.send_string(title);


    cout << "Enter author of the article" << endl;
    string author;
    getline(cin, author);
    cmdh.send_string(author);

    cout << "Enter text, press Ctrl+D/Ctrl+Z on a new line to finish:" << endl;
    string line, text;

    while (std::getline(std::cin, line)) {
        text += line + "\n";
    }
    cin.clear();
    clearerr(stdin);
    
    cmdh.send_string(text);
    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_CREATE_ART) {
        if (acked(cmdh)) {
            cout << "Article created" << endl;
        }
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void delete_article(CommandHandler cmdh) {
    cmdh.send_command(Protocol::COM_DELETE_ART);

    cout << "Enter ID of the news group you wish to delete an article from" << endl;
    cin.ignore();

    string ngIdString;
    getline(cin, ngIdString);
    int ngId = std::stoi(ngIdString);
    cmdh.send_int(ngId);

    cout << "Enter ID of the article you wish to delete" << endl;
    string articleIdString;
    getline(cin, articleIdString);
    int articleId = std::stoi(articleIdString);
    cmdh.send_int(articleId);

    cmdh.send_command(Protocol::COM_END);

    if (cmdh.receive_command() == Protocol::ANS_DELETE_ART) {
        if (acked(cmdh)) {
            cout << "Article with ID " << articleId << " is now deleted." << endl;
        }
    } else {
        cmdh.throw_protocol_error();
    }
    cmdh.ans_ended();
}

void list_commands()
{
    cout << "Welcome. Choose one of the following commands to proceed." << endl;
    cout << "1: List news groups." << endl;
    cout << "2: List articles in news group." << endl;
    cout << "3: Read article." << endl;
    cout << "4: Create news group." << endl;
    cout << "5: Write article." << endl;
    cout << "6: Delete news group." << endl;
    cout << "7: Delete article." << endl;
    cout << "8: Exit." << endl;
    cout << "9: List available commands." << endl;
}

int main(int argc, char* argv[])
{
        std::shared_ptr<Connection> conn = init(argc, argv);
        CommandHandler cmdh(conn);

        list_commands();

        int nbr;
        while (true)
        {
            try
            {
                if (cin >> nbr)
                {

                    switch (nbr)
                    {
                        case 1:
                            list_ng(cmdh);
                            break;
                        case 2:
                            list_articles(cmdh);
                            break;
                        case 3: 
                            read_article(cmdh);
                            break;
                        case 4: 
                            create_ng(cmdh);
                            break;
                        case 5: 
                            write_article(cmdh);
                            break;
                        case 6: 
                            delete_ng(cmdh);
                            break;
                        case 7:
                            delete_article(cmdh);
                            break;
                        case 8: 
                            cout << "exiting..." << endl;
                            return 0;
                        case 9:
                            list_commands();
                            break;
                    }
                }
                else
                {
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // slackfråga
                    cout << "Input does not correspond to an existing command, try again." << endl;

                }
            }
            catch (ConnectionClosedException &)
            {
                cout << "Connection error. Exiting..." << endl;
                return 1;
            }
        }
}
