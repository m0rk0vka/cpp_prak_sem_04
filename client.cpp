#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include "parser.h"

#define PORT 5400


void manual() {
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;
    std::cout << "Description of the model language:" << std::endl;
    std::cout << "SQL-sentence    ::= SELECT or INSERT or UPDATE or DELETE or CREATE or DROP sentence" << std::endl;
    std::cout << "SELECT-sentence ::= SELECT <list of fields> FROM <table name> <WHERE-clause>" << std::endl;
    std::cout << "INSERT-sentence ::= INSERT INTO <table name> (<field value> {, <field value> })" << std::endl;
    std::cout << "UPDATE-sentence ::= UPDATE <table name> SET <field name> = <expression> <WHERE-clause>" << std::endl;
    std::cout << "DELETE-sentence ::= DELETE FROM <table name> <WHERE-clause>" << std::endl;
    std::cout << "CREATE-sentence ::= CREATE TABLE <table name> ( <list of fields descriptions> )" << std::endl;
    std::cout << "DROP-sentence   ::= DROP TABLE <table name>" << std::endl;
    std::cout << std::endl;
    std::cout << "List of fields    ::= <field description> { , <field description> }" << std::endl;
    std::cout << "Field description ::= <field name> <field type>" << std::endl;
    std::cout << "Field type        ::= TEXT ( <unsigned long> ) | LONG" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    std::cout << "cool" << std::endl;
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
        } catch (const std::logic_error& e){
            std::cerr << "Wrong input! Error mesage: " << e.what() << "!" << std::endl;
            std::cerr << "Read manual using \"man\" and write again your request!" << std::endl;
            if (lexer::cur_lex_type != lex_type_t::END && lexer::c != '\n')
            {
                str_cin.clear(); //clears the error flag on cin
                str_cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skips to the next newline
            }
            continue;
        }
        //      Send to server
        try {
            if (parser::request_type == "SELECT") {
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_select.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_select.name.data(), table_name_len + 1, 0);
                int vec_len = parser::request_select.fields.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_select.fields[i].size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_select.fields[i].data(), field_len + 1, 0);
                }
            } else if (parser::request_type == "INSERT") {
                parser::where_clause_type = "NO";
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_insert.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_insert.name.data(), table_name_len + 1, 0);
                int vec_len = parser::request_insert.fields_str.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_insert.fields_str[i].size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_insert.fields_str[i].data(), field_len + 1, 0);
                }
                vec_len = parser::request_insert.fields_num.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    send(sock, &parser::request_insert.fields_num[i], sizeof(long), 0);
                }
                vec_len = parser::request_insert.flags.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    send(sock, &parser::request_insert.flags[i], sizeof(int), 0);
                }
            } else if (parser::request_type == "UPDATE") {
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_update.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_update.name.data(), table_name_len + 1, 0);
                int field_len = parser::request_update.name.size();
                send(sock, &field_len, sizeof(int), 0);
                send(sock, parser::request_update.name.data(), field_len + 1, 0);
                int vec_len = parser::request_update.expression.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len = parser::request_update.expression[i].size();
                    send(sock, &expression_len, sizeof(int), 0);
                    send(sock, parser::request_update.expression[i].data(), expression_len + 1, 0);
                }
            } else if (parser::request_type == "DELETE") {
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_delete.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_delete.name.data(), table_name_len + 1, 0);
            } else if (parser::request_type == "CREATE") {
                parser::where_clause_type = "NO";
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_create.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_create.name.data(), table_name_len + 1, 0);
                int vec_len = parser::request_create.fields_description.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_create.fields_description[i].field.size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_create.fields_description[i].field.data(), field_len + 1, 0);
                    send(sock, &parser::request_create.fields_description[i].size, sizeof(long), 0);
                }
            } else if (parser::request_type == "DROP") {
                parser::where_clause_type = "NO";
                send(sock, parser::request_type.data(), parser::request_type.size() + 1, 0);
                send(sock, parser::where_clause_type.data(), parser::where_clause_type.size() + 1, 0);
                int table_name_len = parser::request_drop.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_drop.name.data(), table_name_len + 1, 0);
            }
//          WHERE-clause
            if (parser::where_clause_type == "LIKE") {
                int field_name_len = parser::like_where_clause.field_name.size();
                send(sock, &field_name_len, sizeof(int), 0);
                send(sock, parser::like_where_clause.field_name.data(), field_name_len + 1, 0);
                send(sock, &parser::like_where_clause.use_not, sizeof(bool), 0);
                int str_len = parser::like_where_clause.sample_string.size();
                send(sock, &str_len, sizeof(int), 0);
                send(sock, parser::like_where_clause.sample_string.data(), str_len + 1, 0);
            } else if (parser::where_clause_type == "IN") {
                int vec_len = parser::in_where_clause.expression.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len = parser::in_where_clause.expression[i].size();
                    send(sock, &expression_len, sizeof(int), 0);
                    send(sock, parser::in_where_clause.expression[i].data(), expression_len + 1, 0);
                }
                send(sock, &parser::in_where_clause.use_not, sizeof(bool), 0);
                vec_len = parser::in_where_clause.list_consts_str.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int const_str_len = parser::in_where_clause.list_consts_str[i].size();
                    send(sock, &const_str_len, sizeof(int), 0);
                    send(sock, parser::in_where_clause.list_consts_str[i].data(), const_str_len + 1, 0);
                }
                vec_len = parser::in_where_clause.list_consts_num.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    send(sock, &parser::in_where_clause.list_consts_num[i], sizeof(long), 0);
                }
            } else if (parser::where_clause_type == "BOOL") {
                int vec_len = parser::bool_where_clause.expression.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len = parser::bool_where_clause.expression[i].size();
                    send(sock, &expression_len, sizeof(int), 0);
                    send(sock, parser::bool_where_clause.expression[i].data(), expression_len + 1, 0);
                }
            } else if (parser::where_clause_type == "ALL") {
                // nothing to do
            }
        } catch (const std::system_error & e) {
            std::cerr << "Can't sending data to server. Error message: " << e.what() << '!' << std::endl;
            std::cerr << "Try again, please." << std::endl;
            continue;
        }

        //      Wait for response
        memset(buf, 0, 4096);
        try {
            int bytesReceived = recv(sock, buf, 4096, 0);
            //      Display response
            std::cout << "SERVER> " << std::string(buf, bytesReceived) << "\r\n";
        } catch (const std::system_error& e) {
            std::cout << "Can't get server response. Error massage: " << e.what() << std::endl;
            continue;
        }
    } while(true);

    //  Close the socket
    close(sock);

    return 0;
}