#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <string>
#include "table.h"

void struct_select::clear() {
    name.clear();
    fields.clear();
}

void struct_insert::clear() {
    name.clear();
    fields_str.clear();
    fields_num.clear();
    flags.clear();
}

void struct_update::clear() {
    name.clear();
    field.clear();
    expression.clear();
}

void struct_delete::clear() {
    name.clear();
}

void struct_create::clear() {
    name.clear();
    fields_description.clear();
}

void struct_drop::clear() {
    name.clear();
}

void struct_like_where_clause::clear() {
    field_name.clear();
    use_not = false;
    sample_string.clear();
}

void struct_in_where_clause::clear() {
    expression.clear();
    use_not = false;
    list_consts_str.clear();
    list_consts_num.clear();
}

void struct_bool_where_clause::clear() {
    expression.clear();
}

Column_struct::Column_struct(int size) {
    columns = new Column[size];
    _size = size;
}

Column_struct::Column_struct(const Column_struct & col) {
    _size = col._size;
    columns = new Column [_size];
    for (int i = 0; i < _size; i++)
        columns[i] = col.columns[i];
}

Column_struct & Column_struct::operator=(const Column_struct & col) {
    _size = col._size;
    columns = new Column [_size];
    for (int i = 0; i < _size; i++)
        columns[i] = col.columns[i];
    return *this;
}

Column_struct::~Column_struct() {
    if(_size)
        delete [] columns;
}

Column & Column_struct::operator[](int index) const {
    return columns[index];
}

int Column_struct::field_id(std::string str) const {
    int id = 0;
    int i = 0;
    while(!id && (i < _size)) {
        if(str == columns[i].name)
            id = i + 1;
        i++;
    }
    return id;
}

std::string Table::next_word(const char* & str) {
    int c = *str++;
    std::string word;
    enum lex_t {H, STR, W, OK} state = H;
    while(state != OK) {
        switch (state) {
        case H:
            if(!std::isspace(c)) {
                if(c == '\'') {
                    state = STR;
                } else {
                    state = W;
                    word += c;
                }
            }
            break;
        case STR:
            if(c == '\'') {
                state = OK;
            } else {
                word += c;
            }
            break;
        case W:
            if(std::isspace(c)) {
                state = OK;
            } else {
                word += c;
            }
            break;
        case OK:
            break;
        }
        if(state != OK) {
            c = *str++;
        }
    }
    return word;
}

Table::Table(std::string _file) {
    name = _file;
    const char* word;
    file.open(_file, std::ios::in);
    if(!file.is_open())
        throw std::logic_error("Server Error: can't open the table");
    std::getline(file, header);
    header += '\n';
    const char* str_ = header.data();
    size = atoi(next_word(str_).data());
    columns = Column_struct(size);
    for (int i = 0; i < size; i++) {
        columns[i].name = next_word(str_);
        word = next_word(str_).data();
        if(word[0] == 'T') {
            word += 1;
            columns[i].type = true;
            columns[i].size = atoi(word);
        } else {
            columns[i].type = false;
        }
    }
}

Table::~Table() {
    file.close();
}

const Column_struct Table::get_struct() {
    return columns;
}

int Table::get_size() {
    return size;
}

void Table::get_record(Record & record, const char* str) {
    for(int i = 0; i < size; i++)
        record[i] = next_word(str);
}

void Table::ddelete() {
    Record record(size);
    std::ofstream fout;
    fout.open(name, std::ios::out);
    fout << header;
    std::string str;
    std::getline(file, str);
    str += '\n';
    while(!file.eof()) {
        get_record(record, str.data());
        //if(!where(record))
            fout << str + '\n';
        std::getline(file, str);
        str += '\n';
    }
    fout.close();
}

void Table::select(std::string & answer, std::vector<int> & id_s) {
    Record record(size);
    std::string str;
    std::getline(file, str);
    str += '\n';
    for(int i = 0; i < id_s.size(); i++) {
        answer += columns[id_s[i]].name + ' ';
    }
    answer += "\n-----------------\n";
    while(!file.eof()) {
        get_record(record, str.data());
        //if(where(record)) {
            for(int i = 0; i < id_s.size(); i++) {
                answer += record[id_s[i]] + ' ';
            }
            answer += '\n';
        //}
        std::getline(file, str);
        str += '\n';
    }
}
