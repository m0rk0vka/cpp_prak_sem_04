#include <iostream>
#include <string.h>

enum state {WAIT, WASH, END};

std::ostream& os = std::cout;

class CarWash{
    state* states;
    const char** num;
    int size;
    bool* use;
    int count = 0;
public:
    CarWash(int n);
    CarWash(const CarWash& a);
    ~CarWash();
    void changeStatus(const std::string& id, const state& value);
    void go(const std::string& id);
    CarWash& operator=(const CarWash& a);
    int set_number(const std::string& id);
    state& operator()(const std::string& id);
    friend CarWash& operator<<(CarWash& a, const std::string& id);
    friend CarWash& operator<<(CarWash& a, const int& pos);
};

CarWash::CarWash(int n){
    size = n;
    states = new state[n];
    num = new const char*[n];
    use = new bool[n];
    for (int i = 0; i < n; ++i){
        states[i] = WAIT;
        use[i] = false;
        num[i] = 0;
    }
}

CarWash::CarWash(const CarWash& a){
    size = a.size;
    states = new state[size];
    num = new const char*[size];
    use = new bool[size];
    count = a.count;
    for (int i = 0; i < size; ++i){
        states[i] = a.states[i];
        use[i] = a.use[i];
    }
    memcpy(num, a.num, size * sizeof(char*));
}

CarWash::~CarWash(){
    delete [] use;
    delete [] num;
    delete [] states;
}

CarWash& CarWash::operator=(const CarWash& a){
    delete [] use;
    delete [] num;
    delete [] states;
    count = a.count;
    size = a.size;
    states = new state[size];
    num = new const char*[size];
    use = new bool[size];
    for (int i = 0; i < size; ++i){
        states[i] = a.states[i];
        use[i] = a.use[i];
    }
    memcpy(num, a.num, size * sizeof(char*));
    return *this;
}

int CarWash::set_number(const std::string& id) {
    if (count >= size) {
        throw std::logic_error("No space");
    }
    for (int i = 0; i < size; ++i){
        if (use[i] == false){
            num[i] = id.c_str();
            use[i] = true;
            states[i] = WAIT;
            ++count;
            return i;
        }
    }
    throw std::logic_error("");
}

void CarWash::changeStatus(const std::string& id, const state& value){
    if (!(value == END || value == WAIT || value == WASH)){
        throw std::logic_error("Invalid status");
    }
    for (int i = 0; i < size; ++i){
        if (num[i] == id){
            states[i] = value;
            return;
        }
    }
    throw std::logic_error("No such id");
}

void CarWash::go(const std::string& id){
    for (int i = 0; i < size; ++i){
        if (id == num[i]){
            if (states[i] != END){
                throw std::logic_error("Car is dirty");
            }
            states[i] = WAIT;
            use[i] = false;
            num[i] = 0;
            --count;
            return;
        }
    }
    throw std::logic_error("No such id");
}

state& CarWash::operator()(const std::string& id){
    for (int i = 0; i < size; ++i){
        if (id == num[i] && use[i] == true){
            return states[i];
        }
    }
    throw std::logic_error("No such id");
}

CarWash& operator<<(std::ostream& os, CarWash& a) {
    return a;
}

CarWash& operator<<(CarWash& a, const std::string& id){
    for (int i = 0; i < a.size; ++i){
        if (id == a.num[i] && a.use[i] == true){
            os << i << ":" << a.num[i] <<  ":" << a.states[i] << '\n';
            return a;
        }
    }
    throw std::logic_error("No such id");
    return a;
}

CarWash& operator<<(CarWash& a, const int& pos){
    if (a.use[pos] == false){
        throw std::logic_error("Empty pos");
    }
    os << pos << ":" << a.num[pos] << ":" << a.states[pos] << '\n';
    return a;
}

int main(){
    CarWash X(2);
    X.set_number("000");
    X.set_number("111");
    try {
        X.set_number("222");
    } catch(const std::logic_error & e) {
        std::cerr << e.what() << std::endl;
    }
    X.changeStatus("111", WASH);
    X.changeStatus("111", END);
    X.go("111");
    /*try {
        X.changeStatus("111", WAIT);
    } catch (const std::logic_error & e) {
        std::cerr << e.what() << std::endl;
    }*/
    //X.set_number("222");
    try {
        X.go("000");
    } catch(const std::logic_error & e) {
        std::cerr << e.what() << std::endl;
    }
    CarWash Y = X;
    CarWash Z(5);
    Z = X;
    Y.set_number("333");
    Z.set_number("333");
    Y("333") = WASH;
    state cur_state = Z("333");
    std::cout << "{0 - WAIT, 1 - WASH, 3 - END} : cur_state = " << cur_state << std::endl;
    try {
        std::cerr << Y << "333" << '\n' << 0 << '\n';
    } catch (const std::logic_error & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
