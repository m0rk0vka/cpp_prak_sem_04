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
        throw std::logic_error("Can't open the table");
    }
    file.seekg(0, std::ios_base::beg);
}

Table::~Table() {
    file.close();
}

void Table::if_select(std::vector<std::string> & fields, std::string & response) {
    std::string head, tmp, tmp1;
    std::getline(file, head);
    head += '\n';
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<int> field_num;
    int j;
    std::cout << cnt_fields << std::endl;
    if (fields.size() == 0) {//if star
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ' && head[i_tmp] != '\n') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            field_num.push_back(j);
            ++j;
        }
    } else {
        for (int i = 0; i < fields.size(); i++) {
            j = 0;
            tmp = fields[i].data();
            int k = i_tmp;
            while (j < cnt_fields) {
                tmp1.clear();
                while (head[k] != ' ' && head[k] != '\n') {
                    tmp1 += head[k];
                    ++k;
                }
                ++k;
                if (head[k] == 'L') {
                    k += 2;
                } else {
                    while (head[k] != ' ' && head[k] != '\n') {
                        ++k;
                    }
                    ++k;
                }
                if (strcmp(tmp1.data(), tmp.data()) == 0){
                    break;
                }
                ++j;
            }
            if (j == cnt_fields) {//this place don't work
                throw std::logic_error("No such field");
            }
            field_num.push_back(j);
        }
    }
    std::string str;
    response = "Select table from " + table_name + " :\n";
    while (!file.eof()){
        std::getline(file, str);
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        str += '\n';
        for (int i = 0; i < field_num.size(); ++i) {
            int j = 0, k = 0;
            while (j <= field_num[i]) {
                tmp.clear();
                while (str[k] != ' ' && str[k] != '\n') {
                    tmp += str[k];
                    ++k;
                }
                ++k;
                ++j;
            }
            response += tmp + ' ';
        }
        response += '\n';
    }
    if (response.size() == 0) {
        throw std::logic_error("Table is empty");
    }
    response.erase(response.end() - 1);
}

void Table::if_select(std::vector<std::string> & fields, struct_like_where_clause & where_clause, std::string & response) {
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
    int j, field_j, cmp = 1;
    if (fields.size() == 0) {//if star
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            if (strcmp(tmp1.data(), where_clause.field_name.data()) == 0){
                cmp = 0;
                field_j = j;
            }
            field_num.push_back(j);
            ++j;
        }
        if (cmp == 1) {
            throw std::logic_error("No such field");
        }
    } else {
        for (int i = 0; i < fields.size(); i++) {
            j = 0;
            tmp = fields[i].data();
            int k = i_tmp;
            while (j < cnt_fields) {
                tmp1.clear();
                while (head[k] != ' ') {
                    tmp1 += head[k];
                    ++k;
                }
                ++k;
                if (head[k] == 'L') {
                    k += 2;
                } else {
                    while (head[k] != ' ') {
                        ++k;
                    }
                    ++k;
                }
                if (strcmp(tmp1.data(), tmp.data()) == 0){
                    break;
                }
                ++j;
            }
            if (j > cnt_fields) {//this place don't work
                throw std::logic_error("No such field");
            }
            field_num.push_back(j);
        }
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            if (strcmp(tmp1.data(), where_clause.field_name.data()) == 0){
                cmp = 0;
                field_j = j;
            }
            ++j;
        }
        if (cmp == 1) {
            throw std::logic_error("No such field");
        }
    }
    std::string str;
    response = "Select table from " + table_name + " :\n";
    while (!file.eof()){
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        int j = 0;
        i_tmp = 0;
        while (str[i_tmp] < str.size()) {
            tmp.clear();
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (field_j == j) {
                break;
            }
            ++j;
        }
        if (if_like(tmp, 0, where_clause.sample_string, 0)) {
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
        }
    }
    if (response.size() == 0) {
        throw std::logic_error("Table is empty");
    }
}

void Table::if_select(std::vector<std::string> & fields, struct_in_where_clause & where_clause, std::string & response) {
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
    std::vector<std::string> fields_vec;
    int j;
    if (fields.size() == 0) {//if star
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            field_num.push_back(j);
            fields_vec.push_back(tmp1.data());
            ++j;
        }
    } else {
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            fields_vec.push_back(tmp1.data());
            ++j;
        }
        for (int i = 0; i < fields.size(); i++) {
            j = 0;
            tmp = fields[i].data();
            int k = i_tmp;
            while (j < cnt_fields) {
                tmp1.clear();
                while (head[k] != ' ') {
                    tmp1 += head[k];
                    ++k;
                }
                ++k;
                if (head[k] == 'L') {
                    k += 2;
                } else {
                    while (head[k] != ' ') {
                        ++k;
                    }
                    ++k;
                }
                if (strcmp(tmp1.data(), tmp.data()) == 0){
                    break;
                }
                ++j;
            }
            if (j > cnt_fields) {//this place don't work
                throw std::logic_error("No such field");
            }
            field_num.push_back(j);
        }
    }
    std::string str;
    std::unordered_map<std::string, std::string> tmp_map;
    response = "Select table from " + table_name + " :\n";
    while (!file.eof()){
        str.clear();
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_in(tmp_map, where_clause.expression, where_clause.list_consts_str, where_clause.list_consts_num)) {
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
        }
    }
    if (response.size() == 0) {
        throw std::logic_error("Table is empty");
    }
}

void Table::if_select(std::vector<std::string> & fields, struct_bool_where_clause & where_clause, std::string & response) {
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
    std::vector<std::string> fields_vec;
    int j;
    if (fields.size() == 0) {//if star
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            field_num.push_back(j);
            fields_vec.push_back(tmp1.data());
            ++j;
        }
    } else {
        j = 0;
        while (j < cnt_fields) {
            tmp1.clear();
            while (head[i_tmp] != ' ') {
                tmp1 += head[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (head[i_tmp] == 'L') {
                i_tmp += 2;
            } else {
                while (head[i_tmp] != ' ') {
                    ++i_tmp;
                }
                ++i_tmp;
            }
            fields_vec.push_back(tmp1.data());
            ++j;
        }
        for (int i = 0; i < fields.size(); i++) {
            j = 0;
            tmp = fields[i].data();
            int k = i_tmp;
            while (j < cnt_fields) {
                tmp1.clear();
                while (head[k] != ' ') {
                    tmp1 += head[k];
                    ++k;
                }
                ++k;
                if (head[k] == 'L') {
                    k += 2;
                } else {
                    while (head[k] != ' ') {
                        ++k;
                    }
                    ++k;
                }
                if (strcmp(tmp1.data(), tmp.data()) == 0){
                    break;
                }
                ++j;
            }
            if (j > cnt_fields) {//this place don't work
                throw std::logic_error("No such field");
            }
            field_num.push_back(j);
        }
    }
    std::string str;
    std::unordered_map<std::string, std::string> tmp_map;
    response = "Select table from " + table_name + " :\n";
    while (!file.eof()){
        str.clear();
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_bool(tmp_map, where_clause.expression)) {
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
        }
    }
    if (response.size() == 0) {
        throw std::logic_error("Table is empty");
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
        throw std::logic_error("Bad input");
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
                i_tmp += 2;
            }
            long num = fields_num[i_num];
            str += std::to_string(num) + " ";
            ++i_num;
        } else {
            std::string text = fields_str[i_str].data();
            if (head[i_tmp] == 'L') {
                throw std::logic_error("Bad field type");
            } else {
                while (head[i_tmp] != ' ') {
                    tmp += head[i_tmp];
                    ++i_tmp;
                }
                ++i_tmp;
                if (atoi(tmp.data()) < text.size() - 2) {
                    throw std::logic_error("Too big TEXT");
                }
            }
            str += text + " ";
            ++i_str;
        }
    }
    str += "\n";
    std::ofstream fout;
    fout.open(table_name, std::ios_base::out | std::ios_base::app);
    fout << str;
    fout.flush();
    fout.close();
    response = "Insert to " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int cmp = 1;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        if (strcmp(tmp1.data(), field.data()) == 0){
            cmp = 0;
        }
        ++j;
    }
    if (cmp == 1) {
        throw std::logic_error("No such field");
    }
    std::string str;
    std::stack<long> tmp_stack;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
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
                tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
            } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
                tmp_stack.push(strtol(item.data(), nullptr, 10));
            } else if (item == "+") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 + op2);
            } else if (item == "-") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 - op2);
            } else if (item == "*") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 * op2);
            } else if (item == "/") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 / op2);
            } else {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 % op2);
            }
        }
        tmp_map[field.data()] = std::to_string(tmp_stack.top());
        tmp_stack.pop();
        std::string str_tmp = "";
        for (int i = 0; i < fields_vec.size(); ++i) {
            str_tmp += tmp_map[fields_vec[i]] + " ";
        }
        str_tmp += '\n';
        fputs(str_tmp.data(), tmp_file);
    }

    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Update " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, struct_like_where_clause & where_clause, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0, field_j;
    int cmp = 1, cmp1 = 1;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        if (strcmp(tmp1.data(), field.data()) == 0){
            cmp = 0;
        }
        if (strcmp(tmp1.data(), where_clause.field_name.data()) == 0){
            cmp1 = 0;
            field_j = j;
        }
        ++j;
    }
    if (cmp == 1 || cmp1 == 1) {
        throw std::logic_error("No such field");
    }
    std::string str;
    std::stack<long> tmp_stack;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        int j = 0;
        i_tmp = 0;
        while (str[i_tmp] < str.size()) {
            tmp.clear();
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (field_j == j) {
                break;
            }
            ++j;
        }
        if (if_like(tmp, 0, where_clause.sample_string, 0)) {
            tmp_map.clear();
            int k = 0;
            i_tmp = 0;
            while (k < cnt_fields) {
                tmp = "";
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
                    tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
                } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
                    tmp_stack.push(strtol(item.data(), nullptr, 10));
                } else if (item == "+") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 + op2);
                } else if (item == "-") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 - op2);
                } else if (item == "*") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 * op2);
                } else if (item == "/") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 / op2);
                } else {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 % op2);
                }
            }
            tmp_map[field.data()] = std::to_string(tmp_stack.top());
            tmp_stack.pop();
            std::string str_tmp = "";
            for (int i = 0; i < fields_vec.size(); ++i) {
                str_tmp += tmp_map[fields_vec[i]] + " ";
            }
            str_tmp += '\n';
            fputs(str_tmp.data(), tmp_file);
        }
    }

    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Update " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, struct_in_where_clause & where_clause, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int cmp = 1;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        if (strcmp(tmp1.data(), field.data()) == 0){
            cmp = 0;
        }
        ++j;
    }
    if (cmp == 1) {
        throw std::logic_error("No such field");
    }
    std::string str;
    std::stack<long> tmp_stack;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_in(tmp_map, where_clause.expression, where_clause.list_consts_str, where_clause.list_consts_num)) {
            for (const std::string & item : expression) {
                if (isalpha(item[0]) || item[0] == '_') {
                    tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
                } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
                    tmp_stack.push(strtol(item.data(), nullptr, 10));
                } else if (item == "+") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 + op2);
                } else if (item == "-") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 - op2);
                } else if (item == "*") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 * op2);
                } else if (item == "/") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 / op2);
                } else {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 % op2);
                }
            }
            tmp_map[field.data()] = std::to_string(tmp_stack.top());
            tmp_stack.pop();
            std::string str_tmp = "";
            for (int i = 0; i < fields_vec.size(); ++i) {
                str_tmp += tmp_map[fields_vec[i]] + " ";
            }
            str_tmp += '\n';
            fputs(str_tmp.data(), tmp_file);
        }
    }

    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Update " + table_name + " was successful.";
}

void Table::if_update(std::string & field, std::vector<std::string> & expression, struct_bool_where_clause & where_clause, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int cmp = 1;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        if (strcmp(tmp1.data(), field.data()) == 0){
            cmp = 0;
        }
        ++j;
    }
    if (cmp == 1) {
        throw std::logic_error("No such field");
    }
    std::string str;
    std::stack<long> tmp_stack;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        str += '\n';
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_bool(tmp_map, where_clause.expression)) {
            for (const std::string & item : expression) {
                if (isalpha(item[0]) || item[0] == '_') {
                    tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
                } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
                    tmp_stack.push(strtol(item.data(), nullptr, 10));
                } else if (item == "+") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 + op2);
                } else if (item == "-") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 - op2);
                } else if (item == "*") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 * op2);
                } else if (item == "/") {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 / op2);
                } else {
                    long op2 = tmp_stack.top();
                    tmp_stack.pop();
                    long op1 = tmp_stack.top();
                    tmp_stack.pop();
                    tmp_stack.push(op1 % op2);
                }
            }
            tmp_map[field.data()] = std::to_string(tmp_stack.top());
            tmp_stack.pop();
            std::string str_tmp = "";
            for (int i = 0; i < fields_vec.size(); ++i) {
                str_tmp += tmp_map[fields_vec[i]] + " ";
            }
            str_tmp += '\n';
            fputs(str_tmp.data(), tmp_file);
        }
    }

    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Update " + table_name + " was successful.";
}

void Table::if_delete(std::string & response) {
    FILE* tmp_file = std::tmpfile();
    std::string head;
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Delete from " + table_name + " was successful.";
}

void Table::if_delete(struct_like_where_clause & where_clause, std::string & response) {
    FILE* tmp_file = std::tmpfile();
    std::string head;
    std::getline(file, head);
    head += '\n';
    std::string tmp;
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        ++i_tmp;
    }
    ++i_tmp;
    bool flag = false;
    int j = 0;
    while (i_tmp < head.size()) {
        tmp.clear();
        while (head[i_tmp] != ' ') {
            tmp += head[i_tmp];
            ++i_tmp;
        }
        ++i_tmp;
        if (strcmp(tmp.data(), where_clause.field_name.data()) == 0) {
            flag = true;
        }
        if (head[i_tmp] == 'L') {
            i_tmp += 2;
            if (flag) {
                throw std::logic_error("Field type in WHERE-clause must be TEXT");
            }
        } else {
            while (head[i_tmp] != ' ') {
                ++i_tmp;
            }
            ++i_tmp;
        }
        if (!flag) {
            ++j;
        }
    }
    if (!flag) {
        throw std::logic_error("No such field");
    }
    fputs(head.data(), tmp_file);
    std::string str;
    while (!file.eof()) {
        std::getline(file, str);
        str += '\n';
        int k = 0;
        i_tmp = 0;
        while (i_tmp < str.size()) {
            tmp.clear();
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            if (k == j) {
                break;
            } else {
                ++k;
            }
        }
        if (if_like(tmp, 0, where_clause.sample_string, 0)) {
            fputs(str.data(), tmp_file);
        }
    }
    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Delete from " + table_name + " was successful.";
}

void Table::if_delete(struct_in_where_clause & where_clause, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        ++j;
    }
    std::string str;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_in(tmp_map, where_clause.expression, where_clause.list_consts_str, where_clause.list_consts_num)) {
            str += '\n';
            fputs(str.data(), tmp_file);
        }
    }
    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Delete from " + table_name + " was successful.";
}

void Table::if_delete(struct_bool_where_clause & where_clause, std::string & response) {
    std::string head, tmp, tmp1;
    FILE* tmp_file = std::tmpfile();
    std::getline(file, head);
    head += '\n';
    fputs(head.data(), tmp_file);
    int i_tmp = 0;
    while (head[i_tmp] != ' ') {
        tmp += head[i_tmp];
        ++i_tmp;
    }
    ++i_tmp;
    int cnt_fields = atoi(tmp.data());
    std::vector<std::string> fields_vec;
    int j = 0;
    int k = i_tmp;
    while (j < cnt_fields) {
        tmp1.clear();
        while (head[k] != ' ') {
            tmp1 += head[k];
            ++k;
        }
        ++k;
        if (head[k] == 'L') {
            k += 2;
        } else {
            while (head[k] != ' ') {
                ++k;
            }
            ++k;
        }
        fields_vec.push_back(tmp1.data());
        ++j;
    }
    std::string str;
    std::unordered_map<std::string, std::string> tmp_map;
    while (!file.eof()) {
        str.clear();
        std::getline(file, str);
        if (strcmp(str.data(), "") == 0) {
            break;
        }
        tmp_map.clear();
        int k = 0;
        i_tmp = 0;
        while (k < cnt_fields) {
            tmp = "";
            while (str[i_tmp] != ' ') {
                tmp += str[i_tmp];
                ++i_tmp;
            }
            ++i_tmp;
            tmp_map[fields_vec[k]] = tmp;
            ++k;
        }
        if (if_bool(tmp_map, where_clause.expression)) {
            str += '\n';
            fputs(str.data(), tmp_file);
        }
    }
    std::ofstream fout;
    fout.open(table_name, std::ios_base::trunc);
    int c;
    std::rewind(tmp_file);
    while ((c = fgetc(tmp_file)) != EOF) {
        fout.put(c);
    }
    fout.flush();
    fout.close();
    std::fclose(tmp_file);
    response = "Delete from " + table_name + " was successful.";
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
        std::logic_error("Can't drop table");
    } else {
        response = "Drop " + table_name + " was successful.";
    }
}

bool Table::if_like(std::string & str, int i_str, std::string & sample_string, int i_smpl_str) {
    int i_tmp1 = i_str, i_tmp2 = i_smpl_str, len_str = str.size(), len_smpl_str = sample_string.size();
    while (i_tmp1 < len_str && i_tmp2 < len_smpl_str) {
        if (sample_string[i_smpl_str] == '%') {
            ++i_smpl_str;
            if (i_tmp2 == len_smpl_str) {
                return true;
            }
            while (i_tmp1 < len_str) {
                if (if_like(str, i_tmp1, sample_string, i_tmp2)) {
                    return true;
                }
                ++i_str;
            }
            return false;
        } else if (sample_string[i_smpl_str] == '_') {
            ++i_str;
            ++i_smpl_str;
        } else if (sample_string[i_smpl_str] == '[') {
            ++i_smpl_str;
            if (sample_string[i_smpl_str] == '^') {
                ++i_smpl_str;
                if (sample_string[i_smpl_str + 1] == '-') {
                    i_smpl_str += 2;
                    if (str[i_str] >= sample_string[i_smpl_str - 2] && str[i_str] <= sample_string[i_smpl_str]) {
                        return false;
                    }
                    ++i_str;
                    i_smpl_str += 2;
                } else {
                    while (sample_string[i_smpl_str] != ']') {
                        if (str[i_str] == sample_string[i_smpl_str]) {
                            return false;
                        }
                        ++i_smpl_str;
                    }
                    ++i_str;
                    ++i_smpl_str;
                }
            } else {
                if (sample_string[i_smpl_str + 1] == '-') {
                    i_smpl_str += 2;
                    if (str[i_str] < sample_string[i_smpl_str - 2] || str[i_str] > sample_string[i_smpl_str]) {
                        return false;
                    }
                    ++i_str;
                    i_smpl_str += 2;
                } else {
                    bool flag = false;
                    while (sample_string[i_smpl_str] != ']') {
                        if (str[i_str] == sample_string[i_smpl_str]) {
                            flag = true;
                        }
                        ++i_smpl_str;
                    }
                    if (!flag) {
                        return false;
                    }
                    ++i_str;
                    ++i_smpl_str;
                }
            }
        } else {
            if (str[i_str] != sample_string[i_smpl_str]) {
                return false;
            }
            ++i_str;
            ++i_smpl_str;
        }
    }
    if (i_tmp1 == len_str && i_tmp2 == len_smpl_str) {
        return true;
    } /*else if (i_tmp2 != len_smpl_str) {
        if (sample_string[i_smpl_str] == '%') {
            return like(iter1, ++i_smpl_str, len_str, len_smpl_str);
        } else {
            return false;
        }
    }*/
    return false;
}

bool Table::if_in(std::unordered_map<std::string, std::string> & tmp_map, std::vector<std::string> & expression, std::vector<std::string> & list_consts_str, std::vector<long> & list_consts_num) {
    std::stack<long> tmp_stack;
    for (const std::string & item : expression) {
        if (isalpha(item[0]) || item[0] == '_') {
            tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
        } else if (isdigit(item[0]) || (item[0] == '+' && item.size() > 1) || (item[0] == '-' && item.size() > 1)) {
            tmp_stack.push(strtol(item.data(), nullptr, 10));
        } else if (item == "+") {
            long op2 = tmp_stack.top();
            tmp_stack.pop();
            long op1 = tmp_stack.top();
            tmp_stack.pop();
            tmp_stack.push(op1 + op2);
        } else if (item == "-") {
            long op2 = tmp_stack.top();
            tmp_stack.pop();
            long op1 = tmp_stack.top();
            tmp_stack.pop();
            tmp_stack.push(op1 - op2);
        } else if (item == "*") {
            long op2 = tmp_stack.top();
            tmp_stack.pop();
            long op1 = tmp_stack.top();
            tmp_stack.pop();
            tmp_stack.push(op1 * op2);
        } else if (item == "/") {
            long op2 = tmp_stack.top();
            tmp_stack.pop();
            long op1 = tmp_stack.top();
            tmp_stack.pop();
            tmp_stack.push(op1 / op2);
        } else {
            long op2 = tmp_stack.top();
            tmp_stack.pop();
            long op1 = tmp_stack.top();
            tmp_stack.pop();
            tmp_stack.push(op1 % op2);
        }
    }
    long tmp = tmp_stack.top();
    tmp_stack.pop();
    if (std::find(list_consts_num.begin(), list_consts_num.end(), tmp) == list_consts_num.end()) {
        return true;
    } else if (std::find(list_consts_str.begin(), list_consts_str.end(), std::to_string(tmp)) == list_consts_str.end()) {
        return true;
    } else {
        return false;
    }
}

bool Table::if_bool(std::unordered_map<std::string, std::string> & tmp_map, std::vector<std::string> & expression) {
    std::stack<long> tmp_stack;
    bool flag = true;
    for (const std::string & item : expression) {
        if (flag && (item != "AND") && (item != "OR") && (item != "NOT")) {
            if (isalpha(item[0]) || item[0] == '_') {
                tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
            } else if (isdigit(item[0]) || (item[0] == '-' && item.size() > 1) || (item[0] == '+' && item.size() > 1)) {
                tmp_stack.push(strtol(item.data(), nullptr, 10));
            }
            flag = false;
        } else {
            if (((isalpha(item[0])) && (item != "AND") && (item != "OR") && (item != "NOT")) || (item[0] == '_')) {
                tmp_stack.push(strtol(tmp_map[item].data(), nullptr, 10));
            } else if (isdigit(item[0]) || (item[0] == '-' && item.size() > 1) || (item[0] == '+' && item.size() > 1) || item[0] == '\'') {//for what ' ?
                tmp_stack.push(strtol(item.data(), nullptr, 10));
            } else if (item == "+") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 + op2);
            } else if (item == "-") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 - op2);
            } else if (item == "*") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 * op2);
            } else if (item == "/") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 / op2);
            } else if (item == "%") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 % op2);
            } else if (item == "AND") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 && op2);
            } else if (item == "OR") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 || op2);
            } else if (item == "NOT") {
                long op = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(!op);
            } else if (item == "=") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 == op2);
                flag = true;
            } else if (item == ">") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 > op2);
                flag = true;
            } else if (item == "<") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 < op2);
                flag = true;
            } else if (item == ">=") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 >= op2);
                flag = true;
            } else if (item == "<=") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 <= op2);
                flag = true;
            } else if (item == "!=") {
                long op2 = tmp_stack.top();
                tmp_stack.pop();
                long op1 = tmp_stack.top();
                tmp_stack.pop();
                tmp_stack.push(op1 != op2);
                flag = true;
            }
        }
    }
    return tmp_stack.top();
}









