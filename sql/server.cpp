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
    std::string response;
    response.clear();
    char buf[4096];
    int res_recv;

    while (true)
    {
        //Wait for client send data
        std::cout << "Well done, now waiting client message" << std::endl;
        response.clear();
        try {
            memset(buf, 0, 4096);
            int len_buf;
            res_recv = recv(clientSocket, &len_buf, 4, 0);
            if (res_recv == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            res_recv = recv(clientSocket, buf, len_buf, 0);
            if (res_recv == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            request_type = buf;
            std::cout << "req_type = " << request_type << "." << std::endl;
            memset(buf, 0, 4096);
            res_recv = recv(clientSocket, &len_buf, 4, 0);
            if (res_recv == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            res_recv = recv(clientSocket, buf, len_buf, 0);
            if (res_recv == -1) {
                throw std::system_error(std::error_code(), "Recv function finished with error code " + std::to_string(errno));
            }
            where_clause_type = buf;
            std::cout << "where_clause_type = " << where_clause_type << "." << std::endl;
        } catch (const std::system_error & e) {
            std::cout << e.what() << std::endl << "Close server." << std::endl;
            close(clientSocket);
            exit(0);
        }
        try {
            if (request_type == "SELECT") {
                request_select.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_select.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    res_recv = recv(clientSocket, &field_len, 4, 0);
                    res_recv = recv(clientSocket, buf, field_len, 0);
                    request_select.fields.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "INSERT") {
                request_insert.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_insert.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    res_recv = recv(clientSocket, &field_len, 4, 0);
                    res_recv = recv(clientSocket, buf, field_len, 0);
                    request_insert.fields_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    long field_num;
                    res_recv = recv(clientSocket, &field_num, 8, 0);
                    request_insert.fields_num.push_back(field_num);
                }
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int flag;
                    res_recv = recv(clientSocket, &flag, 4, 0);
                    request_insert.flags.push_back(flag);
                }
            } else if (request_type == "UPDATE") {
                request_update.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_update.name.append(buf);
                memset(buf, 0, 4096);
                int field_len;
                res_recv = recv(clientSocket, &field_len, 4, 0);
                res_recv = recv(clientSocket, buf, field_len, 0);
                request_update.field.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    res_recv = recv(clientSocket, &expression_len, 4, 0);
                    res_recv = recv(clientSocket, buf, expression_len, 0);
                    request_update.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "DELETE") {
                request_delete.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_delete.name.append(buf);
                memset(buf, 0, 4096);
            } else if (request_type == "CREATE") {
                request_create.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_create.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    res_recv = recv(clientSocket, &field_len, 4, 0);
                    res_recv = recv(clientSocket, buf, field_len, 0);
                    des_tmp.field.append(buf);
                    memset(buf, 0, 4096);
                    res_recv = recv(clientSocket, &des_tmp.size, 8, 0);
                    request_create.fields_description.push_back(std::move(des_tmp));
                    des_tmp.field.clear();
                }
            } else if (request_type == "DROP") {
                request_drop.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                res_recv = recv(clientSocket, &table_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, table_name_len, 0);
                request_drop.name.append(buf);
                memset(buf, 0, 4096);
            }
            if (where_clause_type == "LIKE") {
                like_where_clause.clear();
                memset(buf, 0, 4096);
                int field_name_len;
                res_recv = recv(clientSocket, &field_name_len, 4, 0);
                res_recv = recv(clientSocket, buf, field_name_len, 0);
                like_where_clause.field_name.append(buf);
                memset(buf, 0, 4096);
                res_recv = recv(clientSocket, &like_where_clause.use_not, 1, 0);
                int str_len;
                res_recv = recv(clientSocket, &str_len, 4, 0);
                res_recv = recv(clientSocket, buf, str_len, 0);
                like_where_clause.sample_string.append(buf);
                memset(buf, 0, 4096);
            } else if (where_clause_type == "IN") {
                in_where_clause.clear();
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    res_recv = recv(clientSocket, &expression_len, 4, 0);
                    res_recv = recv(clientSocket, buf, expression_len, 0);
                    in_where_clause.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                res_recv = recv(clientSocket, &in_where_clause.use_not, 1, 0);
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int const_str_len;
                    res_recv = recv(clientSocket, &const_str_len, 4, 0);
                    res_recv = recv(clientSocket, buf, const_str_len, 0);
                    in_where_clause.list_consts_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    long num;
                    res_recv = recv(clientSocket, &num, 8, 0);
                    in_where_clause.list_consts_num.push_back(num);
                }
            } else if (where_clause_type == "BOOL") {
                bool_where_clause.clear();
                memset(buf, 0, 4096);
                int vec_len;
                res_recv = recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int expression_len;
                    res_recv = recv(clientSocket, &expression_len, 4, 0);
                    res_recv = recv(clientSocket, buf, expression_len, 0);
                    bool_where_clause.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (where_clause_type == "ALL") {
                //nothing to do
            }
        } catch (const std::system_error& e) {
            std::string err = e.what();
            response = "You get an error. Server message: " + err + "\n";
            send(clientSocket, &response, response.size(), 0);
            close(clientSocket);
            exit(0);
        }
        try {
            if (request_type == "SELECT") {
                std::string file_name = request_select.name.data();
                Table table(file_name);
                if (where_clause_type == "ALL") {
                    table.if_select(request_select.fields, response);
                } else {

                }
            } else if (request_type == "INSERT") {
                std::string file_name = request_insert.name.data();
                Table table(file_name);
                table.if_insert(request_insert.fields_str, request_insert.fields_num, request_insert.flags, response);
            } else if (request_type == "UPDATE") {
                std::string file_name = request_update.name.data();
                Table table(file_name);
                if (where_clause_type == "ALL") {
                    table.if_update(request_update.field, request_update.expression, response);
                } else {

                }
            } else if (request_type == "DELETE") {
                std::string file_name = request_delete.name.data();
                Table table(file_name);
                if (where_clause_type == "ALL") {
                    table.if_delete(response);
                } else if (where_clause_type == "LIKE") {
                    table.if_delete(like_where_clause, response);
                } else if (where_clause_type == "IN") {
                    table.if_delete(in_where_clause, response);
                } else if (where_clause_type == "BOOL") {
                    table.if_delete(bool_where_clause, response);
                }
            } else if (request_type == "CREATE") {
                std::cout << "create" << std::endl;
                std::string file_name = request_create.name.data();
                Table table(file_name);
                table.if_create(request_create.fields_description, response);
            } else if (request_type == "DROP") {
                std::string file_name = request_drop.name.data();
                Table table(file_name);
                table.if_drop(response);
            }
        } catch (const std::logic_error & e) {
            std::cout << "catch logic error" << std::endl;
            std::string err = e.what();
            response = "You get an error. Server message: " + err;
            int res_len = response.size();
            int res_send = send(clientSocket, &res_len, sizeof(int), 0);
            if (res_send == -1) {
                std::cout << "send error" << std::endl;
                //throw std::system_error(std::error_code(), "Send function finished with error code " + std::to_string(errno));
            }
            std::cout << "response = " << response.data() << std::endl;
            res_send = send(clientSocket, response.data(), res_len, 0);
            if (res_send == -1) {
                std::cout << "send error" << std::endl;
                //throw std::system_error(std::error_code(), "Send function finished with error code " + std::to_string(errno));
            }
            continue;
        }
        // Echo message back to client
        int res_len = response.size();
        //std::cout << "response = " << response.data() << std::endl;
        send(clientSocket, &res_len, sizeof(int), 0);
        send(clientSocket, response.data(), res_len, 0);
    }
    //goto wait_client;
    // Close the socket
    close(clientSocket);

    return 0;
}
