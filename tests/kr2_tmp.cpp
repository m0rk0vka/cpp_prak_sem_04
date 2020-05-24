#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Logger {
public:
    virtual void log(const std::string & message) = 0;
};

class FileLogger : public Logger {
private:
    std::ofstream file;
public:
    FileLogger(const char *fname);
    ~FileLogger();
    void log(const std::string & message);
};

FileLogger::FileLogger(const char *fname) {
    file.open(fname, std::ios_base::app);
}

FileLogger::~FileLogger() {
    file.close();
}

void FileLogger::log(const std::string & message)
{
    if (file.is_open()) {
        file << message.data() << std::endl;
    }
}

class SwitchLogger : public Logger{
private:
    std::vector<Logger *>elem;
    int elp;
public:
    SwitchLogger();
    void addLogger(Logger *logger);
    void switchNext();
    void log(const std::string & message);
};


SwitchLogger::SwitchLogger(){
    this -> elp = 0;
    this -> switchNext();
}

void SwitchLogger::addLogger(Logger *logger){
    elem.push_back(logger);
}

void SwitchLogger::switchNext(){
    if (elem.empty()){
        elp = -1;
    }
    else {
        if (elp == -1){
            elp = 0;
        }
        else {
            elp++;
        }
    }
}

void SwitchLogger::log(const std::string & message){
    if (elp != -1){
        elem[elp] -> log(message);
    }
}

class StderrLogger{
public:
    void log(std::string message);
};

void StderrLogger::log(std::string message){
    std::cerr << message.data() << std::endl;
}

template <typename L>
class PrefixLogger{
private:
    L *elem;
    std::string str;
    int cnt;
public:
    PrefixLogger(L *tmp, const char *s);
    void log(std::string meassage);
};
template <typename L>
PrefixLogger<L>::PrefixLogger(L *tmp, const char *s){
    this -> elem = tmp;
    this -> elem = s;
    this -> cnt = 0;
}
template <typename L>
void PrefixLogger<L>::log(std::string meassage){
    elem -> log(std::to_string(cnt) + str + meassage);
    cnt++;
}


int main(){
    FileLogger x1("1.txt");
    //Logger* x = &x1;
    FileLogger x2("2.txt");
    SwitchLogger y;
    //PrefixLogger<SwitchLogger> z(s1);
    y.addLogger(&x1);
    y.addLogger(&x2);
    y.switchNext();
    y.log("-----");
    std::string s1 = "stroka";
    x1.log(s1);
    return 0;
}