#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>
#include "table.h"

#define PORT 5400

int main()
{
    int server_fd, clientSocket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((clientSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "accept" << std::endl;

    // While loop: accept and echo message back to client
    std::string request_type;
    struct_select request_select;
    struct_insert request_insert;
    struct_update request_update;
    struct_delete request_delete;
    struct_create request_create;
    struct_drop request_drop;
    struct_field_description des_tmp;
//  to WHERE-clause
    std::string where_clause_type;
    struct_like_where_clause like_where_clause;
    struct_in_where_clause in_where_clause;
    struct_bool_where_clause bool_where_clause;
    struct_all_where_clause all_where_clause;
    where_clause *where_clause_ptr;
    std::string answer;
    char buf[4096];

    while (true)
    {
        //Wait for client send data
        try {
            memset(buf, 0, 4096);
            std::cout << "take req_type" << std::endl;
            recv(clientSocket, buf, 8, 0);
            request_type = buf;
            std::cout << "req_type = " << request_type << " wow" << std::endl;
            std::cout << "take wh_type" << std::endl;
            memset(buf, 0, 4096);
            recv(clientSocket, buf, 8, 0);
            where_clause_type = buf;
            std::cout << "end types" << std::endl;
        } catch (const std::system_error& e) {
            exit(0);
        }
        try {
            if (request_type == "SELECT") {
                request_select.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_select.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    recv(clientSocket, &field_len, 4, 0);
                    recv(clientSocket, buf, field_len, 0);
                    request_select.fields.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "INSERT") {
                request_insert.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_insert.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    recv(clientSocket, &field_len, 4, 0);
                    recv(clientSocket, buf, field_len, 0);
                    request_insert.fields_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    long field_num;
                    recv(clientSocket, &field_num, 8, 0);
                    request_insert.fields_num.push_back(field_num);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int flag;
                    recv(clientSocket, &flag, 4, 0);
                    request_insert.flags.push_back(flag);
                }
            } else if (request_type == "UPDATE") {
                request_update.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_update.name.append(buf);
                memset(buf, 0, 4096);
                int field_len;
                recv(clientSocket, &field_len, 4, 0);
                recv(clientSocket, buf, field_len, 0);
                request_update.field.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    recv(clientSocket, &expression_len, 4, 0);
                    recv(clientSocket, buf, expression_len, 0);
                    request_update.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "DELETE") {
                request_delete.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_delete.name.append(buf);
                memset(buf, 0, 4096);
            } else if (request_type == "CREATE") {
                std::cout << "prinimai create" << std::endl;
                request_create.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                std::cout << "recv table_name_len" << std::endl;
                recv(clientSocket, buf, table_name_len, 0);
                std::cout << "recv table_name" << std::endl;
                request_create.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                std::cout << "recv before cecle" << std::endl;
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    std::cout << "recv in cycle i = " << i << std::endl;
                    recv(clientSocket, &field_len, 4, 0);
                    recv(clientSocket, buf, field_len, 0);
                    des_tmp.field.append(buf);
                    memset(buf, 0, 4096);
                    recv(clientSocket, &des_tmp.size, 8, 0);
                    request_create.fields_description.push_back(std::move(des_tmp));
                    des_tmp.field.clear();
                }
                std::cout << "zakanchiva prinimat' create" << std::endl;
            } else if (request_type == "DROP") {
                std::cout << "prinimai drop" << std::endl;
                request_drop.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_drop.name.append(buf);
                memset(buf, 0, 4096);
                std::cout << "zakanchiva prinimat' drop" << std::endl;
            }
            if (where_clause_type == "LIKE") {
                like_where_clause.clear();
                memset(buf, 0, 4096);
                int field_name_len;
                recv(clientSocket, &field_name_len, 4, 0);
                recv(clientSocket, buf, field_name_len, 0);
                like_where_clause.field_name.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &like_where_clause.use_not, 1, 0);
                int str_len;
                recv(clientSocket, &str_len, 4, 0);
                recv(clientSocket, buf, str_len, 0);
                like_where_clause.sample_string.append(buf);
                memset(buf, 0, 4096);
                //like_where_clause.check_str();
                where_clause_ptr = &like_where_clause;
            } else if (where_clause_type == "IN") {
                in_where_clause.clear();
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    recv(clientSocket, &expression_len, 4, 0);
                    recv(clientSocket, buf, expression_len, 0);
                    in_where_clause.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &in_where_clause.use_not, 1, 0);
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int const_str_len;
                    recv(clientSocket, &const_str_len, 4, 0);
                    recv(clientSocket, buf, const_str_len, 0);
                    in_where_clause.list_consts_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    long num;
                    recv(clientSocket, &num, 8, 0);
                    in_where_clause.list_consts_num.push_back(num);
                }
                where_clause_ptr = &in_where_clause;
            } else if (where_clause_type == "BOOL") {
                bool_where_clause.clear();
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    recv(clientSocket, &expression_len, 4, 0);
                    recv(clientSocket, buf, expression_len, 0);
                    bool_where_clause.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                where_clause_ptr = &bool_where_clause;
            } else if (where_clause_type == "ALL") {
                where_clause_ptr = &all_where_clause;
            }
        } catch (const std::system_error& e) {
            continue;
        }
        try {
            if (request_type == "SELECT") {

            } else if (request_type == "INSERT") {

            } else if (request_type == "UPDATE") {

            } else if (request_type == "DELETE") {

            } else if (request_type == "CREATE") {
                std::cout << "create" << std::endl;
                std::string file_name = request_create.name.data();
                Table table(file_name);
                table.if_create(request_create.fields_description, answer);
                std::cout << "end create" << std::endl;
            } else if (request_type == "DROP") {
                std::cout << "drop" << std::endl;
                std::string file_name = request_drop.name.data();
                Table table(file_name);
                table.if_drop(answer);
                std::cout << "end drop" << std::endl;
            }
        } catch (const std::logic_error & e) {
            std::string err = e.what();
            answer = "You get an error. Server message: " + err + "\n";
            send(clientSocket, &answer, answer.size(), 0);
            close(clientSocket);
            exit(0);
        }
        // Echo message back to client
        send(clientSocket, &answer, answer.size(), 0);
    }
    //goto wait_client;
    // Close the socket
    close(clientSocket);

    return 0;
}
