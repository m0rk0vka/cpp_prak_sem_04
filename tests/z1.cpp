#include <iostream>
#include <string>

class Ved {
    unsigned long *stud;
    int *mark;
    int num;
public:
    Ved(int);
    ~Ved();
    Ved(const Ved&);
    void set_stud(unsigned long, int);
    void set_mark(int, unsigned long);
    Ved& operator=(const Ved&);
    int& operator[](unsigned long) const;
};

Ved::Ved(int n) {
    stud = new unsigned long[n];
    mark = new int[n];
    for (int i = 0; i < n; i++) {
        stud[i] = 0;
        mark[i] = 0;
    }
    num = n;
}

Ved::~Ved() {
    delete []stud;
    delete []mark;
}

Ved::Ved(const Ved& a) {
    stud = new unsigned long[a.num];
    mark = new int[a.num];
    num = a.num;
    for (int i = 0; i < num; i++) {
        stud[i] = a.stud[i];
        mark[i] = a.mark[i];
    }
}

void Ved::set_stud(unsigned long num_stud, int pos) {
    if (pos < 0 || pos >= num) {
        throw std::logic_error("No such a position");
    }
    stud[pos] = num_stud;
}

void Ved::set_mark(int m, unsigned long num_stud) {
    if (m < 2 || m > 5) {
        throw std::logic_error("Mark less than 2 or greater than 5");
    }
    for (int i = 0; i < num; i++) {
        if (stud[i] == num_stud) {
            mark[i] = m;
            return;
        }
    }
    throw std::logic_error("No such a student number");
}

Ved& Ved::operator=(const Ved& a) {
    delete []stud;
    delete []mark;
    num = a.num;
    stud = new unsigned long[num];
    mark = new int[num];
    for (int i = 0; i < num; i++) {
        stud[i] = a.stud[i];
        mark[i] = a.mark[i];
    }
    return *this;
}

int& Ved::operator[](unsigned long st) const {
    for (int i = 0; i < num; i++) {
        if (stud[i] == st) {
            return mark[i];
        }
    }
    throw std::logic_error("No such a student number");
}

int main() {
    return 0;
}