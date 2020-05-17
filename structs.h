#include <iostream>
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

struct response {
    std::vector<std::vector<std::string> > res;
    void clear();
};

void response::clear() {
    res.clear();
}

/*class Table {
protected:
    std::string filename;
    std::ifstream fin;
    std::ofstream fout;
    std::vector<std::string> head;
    std::unordered_map<std::string, long> head_map;

    Table(const std::string& str);
    ~Table();
};

Table::Table(const std::string& str) : filename(str) {
    fin.open(filename, std::ios_base::in);
    if (!fin.is_open()) {
        throw std::ios_base::failure("File not exists or another file error occurred");
    }
    fout.open(filename, std::ios_base::in | std::ios_base::ate);
    if (!fout.is_open()) {
        throw std::ios_base::failure("File not exists or another file error occurred");
    }
    int len;
    std::string str_tmp;
    std::stringstream ss;
    fin >> len;
    fin.get();
    for (int i = 0; i < len; ++i)
    {
        std::getline(fin, str_tmp);
        ss << str_tmp;
        ss >> str_tmp;
        head.push_back(std::move(str_tmp));
        ss >> str_tmp;
        head_map[head.back()] = std::strtoll(str_tmp.data(), nullptr, 10);
        ss = std::stringstream();
    }
    start = fin.tellg();
}

Table::~Table()
{
    fout.flush();
    fin.close();
    fout.close();
}

class Select : protected Table {
    select_t* sel_ptr;
    where_base* wh_ptr;
public:
    Select(select_t* sel, where_base* ptr);
    response make_select();
    std::vector<long long> get_head();
};

class Insert : protected Table {
    insert* ins_ptr;
public:
    Insert(insert* ins);
    response make_insert();
    std::vector<long long> get_head();
};

class Update : protected Table {
    update* upd_ptr;
    where_base* wh_ptr;
public:
    Update(update* upd, where_base* wh);
    response make_update();
    std::vector<long long> get_head();
};

class Delete : protected Table {
    where_base* wh_ptr;
    FILE* tmp_file;
public:
    Delete(del* del, where_base* wh);
    ~Delete();
    response make_delete();
    std::vector<long long> get_head();
};

class Create {
public:
    static void make_create(create* ptr);
};

class Drop {
public:
    static void make_drop(const std::string& filename);
};*/