#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <string>
#include "table.h"


Table::Table(std::string _file_name) {
    table_name = _file_name;
    file.open(table_name, std::ios::in);
    if(!file.is_open()) {
        throw std::logic_error("Can't open the table");
    }
}

Table::~Table() {
    file.close();
}

void Table::if_create(std::vector<struct_field_description>& field_description, std::string & answer) {
    std::string head;
    for (int i = 0; i < field_description.size(); i++) {
        long field_size = field_description[i].size;
        std::string field_text = field_description[i].field.data();
        head += field_text + " ";
        if (field_size == -1) {
            head += "L ";
        } else {
            head += std::to_string(field_size) + " ";
        }
    }
    head += "\n";
    std::ofstream fout;
    fout.open(table_name, std::ios::out);
    fout << head;
    fout.close();
    answer = "Create " + table_name + " was successful.";
}

void Table::if_drop(std::string & answer) {
    if(remove(table_name)) {
        std::logic_error("Can't remove file");
    } else {
        answer = "Drop " + table_name + " was successful.";
    }
}
