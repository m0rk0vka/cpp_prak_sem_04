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
    int server_fd, clientSocket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    /*if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }*/
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0L)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((clientSocket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
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
            recv(clientSocket, &request_type, 7, 0);
            recv(clientSocket, &where_clause_type, 5, 0);
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
                request_create.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_create.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    int field_len;
                    recv(clientSocket, &field_len, 4, 0);
                    recv(clientSocket, buf, field_len, 0);
                    des_tmp.field.append(buf);
                    memset(buf, 0, 4096);
                    recv(clientSocket, &des_tmp.size, 8, 0);
                    request_create.fields_description.push_back(std::move(des_tmp));
                    des_tmp.field.clear();
                }
            } else if (request_type == "DROP") {
                request_drop.clear();
                memset(buf, 0, 4096);
                int table_name_len;
                recv(clientSocket, &table_name_len, 4, 0);
                recv(clientSocket, buf, table_name_len, 0);
                request_drop.name.append(buf);
                memset(buf, 0, 4096);
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
                std::string file_name = request_select.name.data();
                Table table(file_name);
                const Column_struct columns = table.get_struct();
                std::vector<int> id_s;
                if (request_select.fields.size() == 0) {
                    for(int i = 0; i < table.get_size(); i++) {
                        id_s.push_back(i);
                    }
                } else {
                    for(int i = 0; i < request_select.fields.size(); ++i) {
                        int id = columns.field_id(request_select.fields[i]);
                        if(!id) {
                            throw std::logic_error("Incorrect Select-call: no such field on table: " + request_select.fields[i]);
                        }
                        id_s.push_back(id - 1);
                    }
                }
                //where(table);
                table.select(answer, id_s);
            } else if (request_type == "INSERT") {
                std::string file_name = request_insert.name.data();
                std::string val;
                int size;
                Column_struct columns;
                Table T(file_name);
                size = T.get_size();
                columns = T.get_struct();
                int i = 0, i_str = 0, i_num = 0;
                while(i <= size - 1) {
                    if (request_insert.flags[i] == 1) {
                        if (columns[i].type){
                            throw std::logic_error("Incorrect: argument #" + std::to_string(i) + " should be text");
                        } else {
                            val += std::to_string(request_insert.fields_num[i_num]) + ' ';
                            ++i_num;
                        }
                    } else {
                        if (columns[i].type){
                            throw std::logic_error("Incorrect: argument #" + std::to_string(i) + " should be text");
                        } else {
                            val += '\'' + request_insert.fields_str[i_str] + '\'';
                            ++i_str;
                        }
                    }
                    ++i;
                }
                FILE* fd = fopen(file_name.data(), "a");
                val += "\n";
                fputs(val.data(), fd);
                answer = "Correct insert";
            } else if (request_type == "UPDATE") {
                /*std::string file_name = request_update.name.data();
                Table table(file_name);
                const Column_struct columns = table.get_struct();
                int id = columns.field_id(request_update.field.data());
                if(!id) {
                    throw std::logic_error("Incorrect Update-call: no such field in table : ") + request_update.field.data();
                }
                where(table);
                table.update(request_update.expression);
                answer = "Correct Update-call";*/
            } else if (request_type == "DELETE") {
                std::string file_name = request_delete.name.data();
                Table table(file_name);
                //where(table);
                table.ddelete();
                answer = "Correct Delete-call";
            } else if (request_type == "CREATE") {
                std::string file_name = request_create.name.data();
                std::string header;
                int size = 0;
                for (int i = 0; i < request_create.fields_description.size(); ++i) {
                    size++;
                    if (request_create.fields_description[i].size == -1){
                        header += request_create.fields_description[i].field + "L ";
                    } else {
                        header += "T" + request_create.fields_description[i].field + ' ';
                    }
                }
                FILE* fd_w = fopen(file_name.data(), "w");
                if(fd_w != NULL) {
                    header = std::to_string(size) + ' ' + header + '\n';
                    fputs(header.data(), fd_w);
                    fclose(fd_w);
                    answer =  "Table " + file_name + " is created";
                } else
                    answer = "Database error: Table " + file_name + " is not created";
            } else if (request_type == "DROP") {
                std::string file_name = request_drop.name.data();
                /*if (!unlink(file_name)) {
                    answer = "Table " + file_name + " is deleted";
                } else {
                    switch(errno) {
                    case EACCES :
                        answer = "Incorrect : access denied";
                    case ENOENT :
                        answer = "Incorrect : no such table in database";
                    }
                }*/
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