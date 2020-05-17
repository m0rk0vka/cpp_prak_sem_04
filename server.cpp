#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "structs.h"

using namespace std;

int main()
{
    // Create a socket
    int listening = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listening == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_UNIX;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = inet_addr("127.0.0.1");
    /*int bind_res = */ bind(listening, (sockaddr*)&hint, sizeof(hint));
    /*if (bind_res == -1) {
        std::cerr << "Can't create local address" << std::endl;
        exit(0);
    }*/
//wait_client:
    // Tell Winsock the socket is for listening
    int listen_res = listen(listening, 128);
    if (listen_res == -1) {
        std::cerr << "Can't listen: " << std::error_code() << std::endl;
        exit(0);
    }
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    std::cout << "listen" << std::endl;
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    std::cout << "accept" << std::endl;
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        cout << host << " connected on port " << service << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    close(listening);

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
                //wh_ptr = &like_where_clause;
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
                //wh_ptr = &in_where_clause;
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
                //wh_ptr = &bool_where_clause;
            } else if (where_clause_type == "ALL") {
                //wh_ptr = &wh4;
            }
        } catch (const std::system_error& e) {
            continue;
        }
        // Echo message back to client
        std::string str_end = "Well done!";
        send(clientSocket, &str_end, 11, 0);
    }
    //goto wait_client;
    // Close the socket
    close(clientSocket);

    return 0;
}