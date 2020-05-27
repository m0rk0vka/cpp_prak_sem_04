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
    std::cout << "--------------------------------------------------------------------------------------" << std::endl;
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
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
        } catch (const std::logic_error& e){
            std::cerr << "Wrong input! Error mesage: " << e.what() << "!" << std::endl;
            std::cerr << "Read manual using \"man\" and write again your request!" << std::endl;
            if (lexer::cur_lex_type != lex_type_t::END && lexer::c != '\n') {
                str_cin.clear(); //clears the error flag on cin
                str_cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skips to the next newline
            }
            continue;
        }
        //      Send to server
        try {
            if (parser::request_type == "SELECT") {
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_select.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_select.name.data(), table_name_len, 0);
                int vec_len = parser::request_select.fields.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_select.fields[i].size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_select.fields[i].data(), field_len, 0);
                }
            } else if (parser::request_type == "INSERT") {
                parser::where_clause_type = "NO";
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_insert.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_insert.name.data(), table_name_len, 0);
                int vec_len = parser::request_insert.fields_str.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_insert.fields_str[i].size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_insert.fields_str[i].data(), field_len, 0);
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
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_update.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_update.name.data(), table_name_len, 0);
                int field_len = parser::request_update.field.size();
                send(sock, &field_len, sizeof(int), 0);
                send(sock, parser::request_update.field.data(), field_len, 0);
                int vec_len = parser::request_update.expression.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len = parser::request_update.expression[i].size();
                    send(sock, &expression_len, sizeof(int), 0);
                    send(sock, parser::request_update.expression[i].data(), expression_len, 0);
                }
            } else if (parser::request_type == "DELETE") {
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_delete.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_delete.name.data(), table_name_len, 0);
            } else if (parser::request_type == "CREATE") {
                parser::where_clause_type = "NO";
                std::cout << parser::request_type.data() << std::endl;
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_create.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_create.name.data(), table_name_len, 0);
                int vec_len = parser::request_create.fields_description.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len = parser::request_create.fields_description[i].field.size();
                    send(sock, &field_len, sizeof(int), 0);
                    send(sock, parser::request_create.fields_description[i].field.data(), field_len, 0);
                    send(sock, &parser::request_create.fields_description[i].size, sizeof(long), 0);
                }
            } else if (parser::request_type == "DROP") {
                parser::where_clause_type = "NO";
                int len_type = parser::request_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::request_type.data(), len_type, 0);
                len_type = parser::where_clause_type.size();
                send(sock, &len_type, sizeof(int), 0);
                send(sock, parser::where_clause_type.data(), len_type, 0);
                int table_name_len = parser::request_drop.name.size();
                send(sock, &table_name_len, sizeof(int), 0);
                send(sock, parser::request_drop.name.data(), table_name_len + 1, 0);
            }
//          WHERE-clause
            if (parser::where_clause_type == "LIKE") {
                int field_name_len = parser::like_where_clause.field_name.size();
                send(sock, &field_name_len, sizeof(int), 0);
                send(sock, parser::like_where_clause.field_name.data(), field_name_len, 0);
                send(sock, &parser::like_where_clause.use_not, sizeof(bool), 0);
                int str_len = parser::like_where_clause.sample_string.size();
                send(sock, &str_len, sizeof(int), 0);
                send(sock, parser::like_where_clause.sample_string.data(), str_len, 0);
            } else if (parser::where_clause_type == "IN") {
                int vec_len = parser::in_where_clause.expression.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len = parser::in_where_clause.expression[i].size();
                    send(sock, &expression_len, sizeof(int), 0);
                    send(sock, parser::in_where_clause.expression[i].data(), expression_len, 0);
                }
                send(sock, &parser::in_where_clause.use_not, sizeof(bool), 0);
                vec_len = parser::in_where_clause.list_consts_str.size();
                send(sock, &vec_len, sizeof(int), 0);
                for (int i = 0; i < vec_len; ++i) {
                    int const_str_len = parser::in_where_clause.list_consts_str[i].size();
                    send(sock, &const_str_len, sizeof(int), 0);
                    send(sock, parser::in_where_clause.list_consts_str[i].data(), const_str_len, 0);
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
                    send(sock, parser::bool_where_clause.expression[i].data(), expression_len, 0);
                }
            } else if (parser::where_clause_type == "ALL" || parser::where_clause_type == "NO") {
                // nothing to do
            }
        } catch (const std::system_error & e) {
            std::cerr << "Can't sending data to server. Error message: " << e.what() << '!' << std::endl;
            std::cerr << "Try again, please." << std::endl;
            continue;
        }

        //      Wait for response
        try {
            std::cout << "waiting response" << std::endl;
            int response_len;
            int res = recv(sock, &response_len, 4, 0);
            if (res == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            memset(buf, 0, 4096);
            res = recv(sock, buf, response_len, 0);
            if (res == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            //      Display response
            std::string response = buf;
            std::cout << "SERVER> " << response.data() << std::endl;
        } catch (const std::system_error& e) {
            std::cout << "Can't get server response. Error massage: " << e.what() << std::endl;
            exit(0);
        }
    } while(true);

    //  Close the socket
    close(sock);

    return 0;
}
