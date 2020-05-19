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
    void if_select();
    void if_insert();
    void if_update();
    void if_delete();
    void if_create(std::vector<struct_field_description> &, std::string &);
    void if_drop();
};
