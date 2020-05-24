#include <iostream>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

struct struct_select {
    std::string name;
    std::vector<std::string> fields;
    void clear();
};

struct struct_insert {
    std::string name;
    std::vector<std::string> fields_str;
    std::vector<long> fields_num;
    std::vector<int> flags; //0 = fields_str; 1 = fields_num;
    void clear();
};

struct struct_update {
    std::string name;
    std::string field;
    std::vector<std::string> expression;
    void clear();
};

struct struct_delete {
    std::string name;
    void clear();
};

struct struct_field_description {
    std::string field;
    long size;
};

struct struct_create {
    std::string name;
    std::vector<struct_field_description> fields_description;
    void clear();
};

struct struct_drop {
    std::string name;
    void clear();
};

struct where_clause {

};

struct struct_like_where_clause : where_clause {
    std::string field_name;
    bool use_not;
    std::string sample_string;
    void clear();
};

struct struct_in_where_clause : where_clause {
    std::vector<std::string> expression;
    bool use_not;
    std::vector<std::string> list_consts_str;
    std::vector<long> list_consts_num;
    void clear();
};

struct struct_bool_where_clause : where_clause {
    std::vector<std::string> expression;
    void clear();
};
