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
    std::vector<std::string> fields;
    void clear();
};

struct struct_update {
    std::string name;
    std::string field;
    std::vector<std::string> expresion;
    void clear();
};

struct struct_delete {
    std::string name;
    void clear();
};

struct struct_create {
    std::string name;
    std::vector<std::string> fields;
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
    fields.clear();
}

void struct_update::clear() {
    name.clear();
    field.clear();
    expresion.clear();
}

void struct_delete::clear() {
    name.clear();
}

void struct_create::clear() {
    name.clear();
    fields.clear();
}

void struct_drop::clear() {
    name.clear();
}