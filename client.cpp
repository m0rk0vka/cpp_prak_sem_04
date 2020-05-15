#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
//#include <string.h>
#include <string>
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
        getline(std::cin, userInput);
        if (userInput == "quit") {
            exit(0);
        }
        if (userInput == "man") {
            manual();
            userInput.clear();
            continue;
        }
        //      clear cin to parser
        for (unsigned int i = 0; i < userInput.size() + 1; ++i) {
            std::cin.unget();
        }
        userInput.clear();
        //      parser work
        try {
            parser::init();
            parser::H();
        }
        catch (const std::logic_error& e){
            std::cerr << "Wrong input! Error mesage: " << e.what() << "!" << std::endl;
            std::cerr << "Read manual using \"man\" and write again your request!" << std::endl;
            /*if (lexer::cur_lex_type != lex_type_t::END && lexer::c != '\n')
            {
                std::cin.clear(); //clears the error flag on cin
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skips to the next newline
            }*/
            continue;
        }
        //      Send to server
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1)
        {
            std::cerr << "Could not send to server! Whoops!\r\n";
            continue;
        }

        //      Wait for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "There was an error getting response from server\r\n";
        }
        else
        {
            //      Display response
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\r\n";
        }
    } while(true);

    //  Close the socket
    close(sock);

    return 0;
}