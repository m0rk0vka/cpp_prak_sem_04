#include <iostream>
#include <vector>
#include <string>

struct select {
    std::string name;
    std::vector<std::string> fields;
    void clear();
};

struct insert {
    std::string name;
    std::vector<std::string> fields;
    void clear();
};

struct update {
    std::string name;
    std::string field;
    std::vector<std::string> expresion;
    void clear();
};

struct delete {
    std::string name;
    void clear();
};

struct create {
    std::string name;
    std::vector<std::string> fields;
    void clear();
};

struct drop() {
    std::string name;
    void clear();
};

void select::clear() {
    name.clear();
    fields.clear();
}

void insert::clear() {
    name.clear();
    fields.clear();
}

void update::clear() {
    name.clear();
    fields.clear();
    expresion.clear();
}

void delete::clear() {
    name.clear();
}

void create::clear() {
    name.clear();
    fields.clear();
}

void drop::clear() {
    name.clear();
}