#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include "structs.h"

using namespace std;

int main()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

//wait_client:
    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    close(listening);

    // While loop: accept and echo message back to client
    std::string request_type;
    struct_select request_select;
    struct_insert request_insert;
    struct_request_updateate request_update;
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
                int len;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_select.name.append(buf);
                memset(buf, 0, 4096);
                int vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    request_select.fields.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "INSERT") {
                request_insert.clear();
                memset(buf, 0, 4096);
                int len, vec_len, int_tmp;
                long long tmp;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_insert.name.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    request_insert.fields_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &tmp, 4, 0);
                    request_insert.fields_num.push_back(tmp);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &int_tmp, 4, 0);
                    request_insert.flags.push_back(int_tmp);
                }
            } else if (request_type == "request_updateATE") {
                request_update.clear();
                memset(buf, 0, 4096);
                int len, vec_len;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_update.name.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_update.field.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    request_update.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
            } else if (request_type == "DELETE") {
                request_delete.clear();
                memset(buf, 0, 4096);
                int len;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_delete.name.append(buf);
                memset(buf, 0, 4096);
            } else if (request_type == "CREATE") {
                request_create.clear();
                memset(buf, 0, 4096);
                int len, vec_len;
                field_def tmp;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_create.name.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    tmp.field.append(buf);
                    memset(buf, 0, 4096);
                    recv(clientSocket, &tmp.size, 4, 0);
                    request_create.fields_def.push_back(std::move(tmp));
                    tmp.field.clear();
                }
            } else if (request_type == "DROP") {
                request_drop.clear();
                memset(buf, 0, 4096);
                int len;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                request_drop.name.append(buf);
                memset(buf, 0, 4096);
            }
            if (where_clause_type == "LIKE") {
                like_where_clause.clear();
                memset(buf, 0, 4096);
                int len;
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                like_where_clause.field.append(buf);
                memset(buf, 0, 4096);
                recv(clientSocket, &like_where_clause.neg, 1, 0);
                recv(clientSocket, &len, 4, 0);
                recv(clientSocket, buf, len, 0);
                like_where_clause.str.append(buf);
                memset(buf, 0, 4096);
                like_where_clause.check_str();
                wh_ptr = &like_where_clause;
            } else if (where_clause_type == "IN") {
                in_where_clause.clear();
                memset(buf, 0, 4096);
                int len, vec_len;
                long long tmp;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    in_where_clause.expression.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &in_where_clause.neg, 1, 0);
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    in_where_clause.list_str.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &tmp, 4, 0);
                    in_where_clause.list_num.push_back(tmp);
                }
                wh_ptr = &in_where_clause;
            } else if (where_clause_type == "BOOL") {
                bool_where_clause.clear();
                memset(buf, 0, 4096);
                int len, vec_len;
                recv(clientSocket, &vec_len, 4, 0);
                for (int i = 0; i < vec_len; ++i) {
                    recv(clientSocket, &len, 4, 0);
                    recv(clientSocket, buf, len, 0);
                    bool_where_clause.logic_expr.push_back(std::string(buf));
                    memset(buf, 0, 4096);
                }
                wh_ptr = &bool_where_clause;
            } else if (where_clause_type == "ALL") {
                wh_ptr = &wh4;
            }
        } catch (const std::system_error& e) {
            continue;
        }
        // Echo message back to client
        //send(clientSocket, buf, bytesReceived + 1, 0);
    }
    //goto wait_client;
    // Close the socket
    close(clientSocket);

    return 0;
}