#include <iostream>
#include <vector>
#include <string>
#include "structs.h"

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