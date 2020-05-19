#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
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

struct struct_all_where_clause : where_clause {

};

struct Column {
    bool type;
    int size;
    std::string name;
};

class Column_struct {
    Column* columns;
    int _size;
public:
    Column_struct() {_size = 0;}
    Column_struct(int size);
    Column_struct(const Column_struct &);
    ~Column_struct();
    Column & operator[](int index) const;
    Column_struct & operator=(const Column_struct & );
    int field_id(std::string) const;
};

class Record {
    std::string* fields;
public:
    Record(int size) {fields = new std::string[size];}
    ~Record() {delete [] fields;}
    std::string & operator[](int index) {return fields[index];}
};

class Table {
    std::string name;
    std::string header;
    std::ifstream file;
    int size;
    Column_struct columns;
    std::string next_word(const char* &);
    /*bool where_re(const char*, const char*);
    bool where(Record &);*/
    void get_record(Record &, const char* );
public:
    Table(std::string);
    ~Table();
    const Column_struct get_struct();
    int get_size();
    /*int where_type;
    int where_id;
    int where_count;
    bool where_not;
    std::string where_str;*/
    void ddelete();
    void update(std::vector<std::string> &);
    void select(std::string &, std::vector<int> &);
};
