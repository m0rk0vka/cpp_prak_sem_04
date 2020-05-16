#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include "parser.h"

void manual()
{
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Description of the model language:" << std::endl;
    std::cout << "SQL-sentence    ::= SELECT or INSERT or UPDATE or DELETE or CREATE or DROP sentence" << std::endl;
    std::cout << "SELECT-sentence ::= SELECT <list of fields> FROM <table name> <WHERE-clause>" << std::endl;
    std::cout << "INSERT-sentence ::= INSERT INTO <table name> (<field value> {, <field value> })" << std::endl;
    std::cout << "UPDATE-sentence ::= UPDATE <table name> SET <field name> = <expression> <WHERE-clause>" << std::endl;
    std::cout << "DELETE-sentence ::= DELETE FROM <table name> <WHERE-clause>" << std::endl;
    std::cout << "CREATE-sentence ::= CREATE TABLE <table name> ( <list of fields definitions> )" << std::endl;
    std::cout << "DROP-sentence   ::= DROP TABLE <table name>" << std::endl;
    std::cout << std::endl;
    std::cout << "List of fields definitions syntax: <field definition> { , <field definition> }" << std::endl;
    std::cout << "Field definition syntax: <field name> <field type>" << std::endl;
    std::cout << "Field type syntax: TEXT ( <unsigned long long> ) | LONG" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    //  Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "Can't create socket" << std::endl;
        exit(0);
    }

    //  Create a hint structure for the server we're connecting with
    int port = 54000;
    std::string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //  Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        std::cerr << "Can't connect to the server" << std::endl;
        exit(0);
    }

    //  While loop:
    char buf[4096];
    std::string userInput;

    std::cout << "Hello! Now you can work with database." << std::endl;
    std::cout << "Print \"man\" to see manual. Print \"quit\" to quit the program. " << std::endl;
    do {
        //      Enter lines of text
        std::cout << "> ";
        userInput.clear();
        std::getline(std::cin, userInput);
        if (userInput == "quit") {
            exit(0);
        }
        if (userInput == "man") {
            manual();
            userInput.clear();
            continue;
        }
        //      clear cin to parser
        str_cin.clear();
        str_cin << userInput << '\n';
        //      parser work
        try {
            parser::init();
            parser::H();
            parser::check_end();
        }
        catch (const std::logic_error& e){
            std::cerr << "Wrong input! Error mesage: " << e.what() << "!" << std::endl;
            std::cerr << "Read manual using \"man\" and write again your request!" << std::endl;
            if (lexer::cur_lex_type != lex_type_t::END && lexer::c != '\n')
            {
                str_cin.clear(); //clears the error flag on cin
                str_cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skips to the next newline
            }
            continue;
        }
        std::cout << "Well\n";
        //      Send to server
        try {
            if (parser::request_type == "SELECT") {
                int len_table_name = parser::request_select.name.size();
                int sendRes = send(sock, parser::request_select.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            } else if (parser::request_type == "INSERT") {
                int len_table_name = parser::request_insert.name.size();
                int sendRes = send(sock, parser::request_insert.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            } else if (parser::request_type == "UPDATE") {
                int len_table_name = parser::request_update.name.size();
                int sendRes = send(sock, parser::request_update.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            } else if (parser::request_type == "DELETE") {
                int len_table_name = parser::request_delete.name.size();
                int sendRes = send(sock, parser::request_delete.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            } else if (parser::request_type == "CREATE") {
                int len_table_name = parser::request_create.name.size();
                int sendRes = send(sock, parser::request_create.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            } else if (parser::request_type == "DROP") {
                int len_table_name = parser::request_drop.name.size();
                int sendRes = send(sock, parser::request_drop.name.data(), len_table_name + 1, 0);
                if (sendRes == -1)
                {
                    std::cerr << "Could not send to server! Whoops!\r\n";
                    continue;
                }
            }
        } catch (const std::logic_error & e) {
            std::cerr << "Can't sending data to server. Error message: " << e.what() << '!' << std::endl;
            std::cerr << "Try again, please." << std::endl;
        }

        //      Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1) {
            std::cerr << "There was an error getting response from server\r\n";
        } else {
            //      Display response
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\r\n";
        }
    } while(true);

    //  Close the socket
    close(sock);

    return 0;
}