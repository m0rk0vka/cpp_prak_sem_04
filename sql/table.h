#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <string>
#include "structs.h"


class Table {
    std::string table_name;
    std::ifstream file;
public:
    Table(std::string);
    ~Table();
    void if_select(std::vector<std::string> &, std::string &);
    void if_select(std::vector<std::string> &, struct_like_where_clause &, std::string &);
    void if_select(std::vector<std::string> &, struct_in_where_clause &, std::string &);
    void if_select(std::vector<std::string> &, struct_bool_where_clause &, std::string &);
    void if_insert(std::vector<std::string> &, std::vector<long> &, std::vector<int> &, std::string &);
    void if_update(std::string &, std::vector<std::string> &, std::string &);
    void if_update(std::string &, std::vector<std::string> &, struct_like_where_clause & , std::string &);
    void if_update(std::string &, std::vector<std::string> &, struct_in_where_clause &, std::string &);
    void if_update(std::string &, std::vector<std::string> &, struct_bool_where_clause &, std::string &);
    void if_delete(std::string &);
    void if_delete(struct_like_where_clause &, std::string &);
    void if_delete(struct_in_where_clause &, std::string &);
    void if_delete(struct_bool_where_clause &, std::string &);
    void if_create(std::vector<struct_field_description> &, std::string &);
    void if_drop(std::string &);
    bool if_like(std::string &, int, std::string &, int);
    bool if_in(std::unordered_map<std::string, std::string> &, std::vector<std::string> &, std::vector<std::string> &, std::vector<long> &);
    bool if_bool(std::unordered_map<std::string, int> &, std::unordered_map<std::string, std::string> &, std::vector<std::string> &);
};
