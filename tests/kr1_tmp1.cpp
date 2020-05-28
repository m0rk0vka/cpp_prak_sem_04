#include <iostream>
#include <string.h>

constexpr size_t MAX_STRING_SIZE = 10;

enum state { WAIT, WASH, END};

class Ved{
    state* states;
    const char** num;
    size_t _size;
    bool* init;
    size_t count = 0;
public:
    Ved(size_t n);
    Ved(const Ved& other);
    ~Ved();
    void change(const state& value, const std::string& id);
    void leave(const std::string& id);
    Ved& operator=(const Ved& other);
    size_t insert_num(const std::string& id);
    state& operator[](const std::string& id);
};

Ved::Ved(size_t n){
    _size = n;
    states = new state[n];
    num = new const char*[n];
    init = new bool[n];
    for (size_t i = 0; i < n; ++i){
        states[i] = WAIT;
        init[i] = false;
        num[i] = 0;
    }
}

Ved::Ved(const Ved& other){
    _size = other._size;
    states = new state[_size];
    num = new const char*[_size];
    init = new bool[_size];
    count = other.count;
    for (size_t i = 0; i < _size; ++i){
        states[i] = other.states[i];
        init[i] = other.init[i];
    }
    memcpy(num, other.num, _size * sizeof(char*));
}

Ved::~Ved(){
    delete [] init;
    delete [] num;
    delete [] states;
}

Ved& Ved::operator=(const Ved& other){
    if (this == &other){
        return *this;
    }
    delete [] init;
    delete [] num;
    delete [] states;
    count = other.count;
    _size = other._size;
    states = new state[_size];
    num = new const char*[_size];
    init = new bool[_size];
    for (size_t i = 0; i < _size; ++i){
        states[i] = other.states[i];
        init[i] = other.init[i];
    }
    memcpy(num, other.num, _size * sizeof(char*));
    return *this;
}

size_t Ved::insert_num(const std::string& id){
    if (count >= _size){
        throw std::logic_error("no space");
    }
    if (id.size() >= MAX_STRING_SIZE){
        throw std::logic_error("invalid number size");
    }
    for (size_t i = 0; i < _size; ++i){
        if (init[i] == false){
            num[i] = id.c_str();
            init[i] = true;
            states[i] = WAIT;
            ++count;
            return i;
        }
    }
    throw std::logic_error("No such id");
}

void Ved::leave(const std::string& id){
    for (size_t i = 0; i < _size; ++i){
        if (id == num[i]){
            if (states[i] != END){
                throw std::logic_error("auto is dirty");
            }
            init[i] = false;
            --count;
            return;
        }
    }
    throw std::logic_error("no such id");
}

void Ved::change(const state& value, const std::string& id){
    if (!(value == END || value == WAIT || value == WASH)){
        throw std::logic_error("invalid status");
    }
    for (size_t i = 0; i < _size; ++i){
        if (id == num[i]){
            states[i] = value;
            return;
        }
    }
    throw std::logic_error("no such id");
}

state& Ved::operator[](const std::string& id){
    for (size_t i = 0; i < _size; ++i){
        if (id == num[i] && init[i] == true){
            return states[i];
        }
    }
    throw std::logic_error("no such id");
}

/*class Transfer{
public:
    Transfer(std::ostream& stream, const Ved& vedom);
    Transfer operator<<(const std::string& str);
    Transfer operator<<(const size_t& pos);
private:
    std::ostream& s;
    Ved v;
    };

Transfer::Transfer(std::ostream& stream, const Ved& vedom) : s(stream), v(vedom){}


Transfer Transfer::operator<<(const std::string& id){
    for (size_t i = 0; i < v._size; ++i){
        if (id == v.num[i] && v.init[i] == true){
            s << i << ":" << v.num[i] << v.states[i] << '\n';
            return *this;
        }
    }
    throw std::logic_error("no such id");
    return *this;
}

Transfer Transfer::operator<<(const size_t& pos){
    if (v.init[pos] == false){
        throw std::logic_error("Free pos");
    }
    s << pos << ":" << v.num[pos] << ":" << v.states[pos] << '\n';
    return *this;
}

Transfer operator<<(std::ostream& s, const Ved& v){
    return Transfer{s, v};
}*/

int main(){
    Ved v(5);
    v.insert_num("DVK");
    v["DVK"] = WASH;
    // std::cout << v << "DVK" << '\n';
}
