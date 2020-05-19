#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
//#include <dos.h>
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

void Table::if_select(std::vector<std::string> & fields, std::string & where_type, std::string & response) {
    //ychest' esli *
    std::string head, tmp, tmp1;
    std::getline(file, head);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<int> field_num;
    int j;
    for (int i = 0; i < fields.size(); i++) {
        j = 0;
        tmp = fields[i].data();
        while (j < cnt_fields) {
            tmp1.clear();
            int k = i_tmp;
            while (head[k] != ' ') {
                tmp1 += head[k];
                ++k;
            }
            ++k;
            if (strcmp(tmp1.data(), tmp.data()) == 0){
                break;
            }
            ++j;
        }
        if (j == cnt_fields) {
            throw std::logic_error("No such field");
        }
        field_num.push_back(j - 1);
    }
    std::string str;
    std::stringstream ss;
    while (true){
        std::getline(file, str);
        //ss << str << '\n';
        if (where_type == "ALL") {
            for (int i = 0; i < field_num.size(); ++i) {
                int j = 0, k = 0;
                while (j <= field_num[i]) {
                    tmp.clear();
                    while (str[k] != ' ') {
                        tmp += str[k];
                        ++k;
                    }
                    ++k;
                    ++j;
                }
                response += tmp + ' ';
            }
            response += '\n';
        } else {
            response = "I don't know how to work with where-clause type = " + tmp + '\n';
        }
    }
}

void Table::if_insert(std::vector<std::string> & fields_str, std::vector<long> & fields_num, std::vector<int> & flags, std::string & response) {
    std::string str, head, tmp;
    std::getline(file, head);
    int i_str = 0, i_num = 0, i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    if (atoi(tmp.data()) != flags.size()) {
        throw std::logic_error("Few input");
    }
    ++i_tmp;
    for (int i = 0; i < flags.size(); i++) {
        tmp.clear();
        while (head[i_tmp] != ' ') {
            ++i_tmp;
        }
        ++i_tmp;
        if (flags[i] == 1){
            if (head[i_tmp] != 'L') {
                throw std::logic_error("Bad field type");
            } else {
                ++i_tmp;
                ++i_tmp;
            }
            long num = fields_num[i_num];
            str += std::to_string(num) + " ";
            ++i_num;
        } else {
            std::string text = fields_str[i].data();
            if (head[i_tmp] == 'L') {
                throw std::logic_error("Bad field type");
            } else {
                while (head[i_tmp] != ' ') {
                    tmp += head[i_tmp];
                    ++i_tmp;
                }
                ++i_tmp;
                if (atoi(tmp.data()) < text.size()) {
                    throw std::logic_error("Too bid TEXT");
                }
            }
            str += text + " ";
            ++i_str;
        }
    }
    str += "\n";
    std::ofstream fout;
    fout.open(table_name, std::ios::out);
    fout << head;
    fout.flush();
    fout.close();
    response = "Insert to " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, std::string & where_type, std::string & response) {

}

void Table::if_delete(std::string & where_type, std::string & response) {
    FILE* tmp_file = std::tmpfile();
    if (where_type == "ALL") {
        std::string head;
        std::getline(file, head);
        head += '\n';
        fputs(head.data(), tmp_file);
        std::ofstream fout;
        fout.open(table_name, std::ios::out);
        int c;
        while ((c = fgetc(tmp_file)) != EOF) {
            fout.put(c);
        }
        fout.flush();
        fout.close();
    }
    std::fclose(tmp_file);
}

void Table::if_create(std::vector<struct_field_description>& field_description, std::string & response) {
    std::string head;
    int size = field_description.size();
    head += std::to_string(size) + " ";
    for (int i = 0; i < size; i++) {
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
    head.clear();
    response = "Create " + table_name + " was successful.";
}

void Table::if_drop(std::string & response) {
    if(remove(table_name.data())) {
        std::logic_error("Can't remove file");
    } else {
        response = "Drop " + table_name + " was successful.";
    }
}
