#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <stdexcept>
#include <cerrno>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <string>
#include "table.h"


Table::Table(std::string _file_name) {
    table_name = _file_name;
    file.open(table_name, std::ios_base::app);
    if(!file.is_open()) {
        std::cout << "lol" << std::endl;
        throw std::logic_error("Can't open the table");
    }
}

Table::~Table() {
    file.close();
}

void Table::if_select(std::vector<std::string> & fields, std::string & where_type, std::string & response) {
    //ychest' esli *
    file.seekg(0, std::ios_base::beg);
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
        if (j > cnt_fields) {//this place don't work
            std::cout << "exit" << std::endl;
            throw std::logic_error("No such field");
        }
        field_num.push_back(j);
    }
    std::string str;
    std::cout << "field_num.size() = " << field_num.size() << std::endl;
    for (int i = 0; i < field_num.size(); ++i) {
        std::cout << field_num[i] << " ";
    }
    std::cout << std::endl;
    while (!file.eof()){
        std::getline(file, str);
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        std::cout << "nach" << str << "kon" << std::endl;
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
            response = "I don't know how to work with where-clause type = " + where_type + '\n';
        }
    }
    if (response.size() == 0) {
        throw std::logic_error("Table is empty");
    }
    std::cout << "response = " << response << std::endl;
}

void Table::if_insert(std::vector<std::string> & fields_str, std::vector<long> & fields_num, std::vector<int> & flags, std::string & response) {
    file.seekg(0, std::ios_base::beg);
    std::string str, head, tmp;
    std::getline(file, head);
    std::cout << head << std::endl;
    int i_str = 0, i_num = 0, i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    std::cout << "getline done, size = " << tmp.data() << " get size = " << flags.size() << std::endl;
    if (atoi(tmp.data()) != flags.size()) {
        throw std::logic_error("Few input");
    }
    ++i_tmp;
    std::cout << "before for" << std::endl;
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
    std::cout << "after for" << std::endl;
    std::ofstream fout;
    fout.open(table_name, std::ios_base::out | std::ios_base::app);
    fout << str;
    fout.flush();
    fout.close();
    std::cout << "end zapic v file" << std::endl;
    response = "Insert to " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, std::string & where_type, std::string & response) {
    std::string head, tmp, tmp1;
    std::getline(file, head);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int cmp = 0;
    tmp = field.data();
    while (j < cnt_fields) {
        tmp1.clear();
        int k = i_tmp;
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        fields_vec.push_back(tmp1.data());
        if (strcmp(tmp1.data(), tmp.data()) == 0){
            cmp = 1;
        }
        ++j;
    }
    if (cmp == 1) {
        throw std::logic_error("No such field");
    }
    --j;
    std::string str;
    std::stack<long> stack_num;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        std::getline(file, str);
        if (where_type == "ALL") {
            tmp_map.clear();
            int k = 0;
            while (k < cnt_fields) {
                tmp.clear();
                i_tmp = 0;
                while (str[i_tmp] != ' ') {
                    tmp += str[i_tmp];
                    ++i_tmp;
                }
                ++i_tmp;
                tmp_map[fields_vec[k]] = tmp;
                ++k;
            }
            for (const std::string & item : expression) {
                if (isalpha(item[0]) || item[0] == '_') {
                    stack_num.push(strtol(tmp_map[item].data(), nullptr, 10));
                } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
                    stack_num.push(strtol(item.data(), nullptr, 10));
                } else if (item == "+") {
                    long op2 = stack_num.top();
                    stack_num.pop();
                    long op1 = stack_num.top();
                    stack_num.pop();
                    stack_num.push(op1 + op2);
                } else if (item == "-") {
                    long op2 = stack_num.top();
                    stack_num.pop();
                    long op1 = stack_num.top();
                    stack_num.pop();
                    stack_num.push(op1 - op2);
                } else if (item == "*") {
                    long op2 = stack_num.top();
                    stack_num.pop();
                    long op1 = stack_num.top();
                    stack_num.pop();
                    stack_num.push(op1 * op2);
                } else if (item == "/") {
                    long op2 = stack_num.top();
                    stack_num.pop();
                    long op1 = stack_num.top();
                    stack_num.pop();
                    stack_num.push(op1 / op2);
                } else {
                    long op2 = stack_num.top();
                    stack_num.pop();
                    long op1 = stack_num.top();
                    stack_num.pop();
                    stack_num.push(op1 % op2);
                }
            }
            tmp_map[field.data()] = std::to_string(stack_num.top());
            stack_num.pop();
            for (auto item : tmp_map) {
                response += item.second + ' ';
            }
            response += '\n';
        }
    }
}

void Table::if_delete(std::string & where_type, std::string & response) {
    FILE* tmp_file = std::tmpfile();
    if (where_type == "ALL") {
        std::string head;
        file.seekg(0, std::ios_base::beg);
        std::getline(file, head);
        head += '\n';
        fputs(head.data(), tmp_file);
        std::ofstream fout;
        fout.open(table_name, std::ios_base::trunc);
        int c;
        std::rewind(tmp_file);
        while ((c = fgetc(tmp_file)) != EOF) {
            std::cout << c;
            fout.put(c);
        }
        fout.flush();
        fout.close();
    }
    std::fclose(tmp_file);
}

void Table::if_create(std::vector<struct_field_description>& field_description, std::string & response) {
    std::cout << "if create start" << std::endl;
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
