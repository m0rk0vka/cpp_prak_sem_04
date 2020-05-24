#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

class Logger {
    public:
        virtual void log(const std::string & message) = 0;
        virtual ~Logger();
};

Logger::~Logger() {};

class FileLogger : public Logger {
    private:
        std::ofstream o;
    public:
        FileLogger(const char *pwd);
        ~FileLogger() override;
        virtual void log(const std::string & message) override;
};

FileLogger::FileLogger(const char *pwd) {
    o.open(pwd, std::ios::app);
}
FileLogger::~FileLogger() {
    o.close();
}
void FileLogger::log(const std::string & message){
    if (o.is_open()) {
        o << message.data() << std::endl;
    }
}

class StderrLogger{
    public:
        void log(std::string message);
};

void StderrLogger::log(std::string message){
    std::cerr << message << std::endl;
}

template <class L>
class PrefixLogger{
    private:
        L logger;
        std::string defs;
        unsigned num;
    public:
        PrefixLogger(std::string s = "");
        void log(std::string message);
};

template<class L>
PrefixLogger<L>::PrefixLogger(std::string s){
    defs = s;
    num = 0;
}

template<class L>
void PrefixLogger<L>::log(std::string message){
    logger.log(std::to_string(num++) + " " + defs + " " + message);
}

class SwitchLogger : public Logger {
    private:
        std::vector<Logger*> logs;
        Logger* cur;
        size_t cur_pos;
    public:
        SwitchLogger();
        void addLogger(Logger *logger);
        virtual void log(const std::string & message) override;
        void switchNext();
};

SwitchLogger::SwitchLogger(){
    cur_pos = -1;
}

void SwitchLogger::addLogger(Logger *logger) {
    logs.push_back(logger);
    if (cur_pos == -1){
        cur = logs[0];
        cur_pos = 0;
    }
}

void SwitchLogger::log(const std::string & message){
    if (cur_pos != -1){
        cur->log(message);
    }
}

void SwitchLogger::switchNext(){
    if ((cur_pos + 2) <= logs.size()){
        cur = logs[++cur_pos];
        return;
    }
    if ((cur_pos + 1) == logs.size()){
        cur_pos = 0;
        cur = logs[cur_pos];
        return;
    }
}

struct Skates {
    std::string size;
    int quantity;
};

template <class T>
class SkatesRent {
    private:
        std::vector<Skates> skates;
        T *logger;
    public:
        SkatesRent(T *logger_init);
        void addSkates(std::string size, int quantity);
        void getSkates(std::string size, int quantity);
        int curSkates(std::string size);
};

template<class T>
SkatesRent<T>::SkatesRent(T *logger_init) {
    logger = logger_init;
}


template<class T>
void SkatesRent<T>::addSkates(std::string size, int quantity) {
    for (int i = 0; i < skates.size(); i++) {
        if (skates[i].size == size) {
            logger->log("Added " + std::to_string(quantity) + " of " + size);
            skates[i].quantity += quantity;
            return;
        }
    }

    skates.push_back(Skates{size, quantity});
    logger->log("Added " + size + " in quantity of " + std::to_string(quantity));
}

template<class T>
void SkatesRent<T>::getSkates(std::string size, int quantity) {
    for (int i = 0 ; i < skates.size(); i++) {
        if (skates[i].size == size) {
            if (skates[i].quantity >= quantity) {
                logger->log("Taken " + size + " in quantity of " + std::to_string(quantity));
                skates[i].quantity -= quantity;
                return;
            } else {
                logger->log("Taken " + size + " in quantity of " + std::to_string(skates[i].quantity) + " didn't have enough skates");
                skates[i].quantity = 0;
                return;
            }
        }
    }
    logger->log("There were no skates like that in rent");
}

template<class T>
int SkatesRent<T>::curSkates(std::string size){
    for (int i = 0 ; i < skates.size(); i++) {
        if (skates[i].size == size) {
            logger->log("Currently " + std::to_string(skates[i].quantity) + " of " + size);
            return skates[i].quantity;
        }
    }
    logger->log("Currently 0 of " + size);
    return 0;
}

int main(void)
{
    std::string data = "Q";
    FileLogger a("asd.txt");
    a.log(data);
    SwitchLogger b;
    b.addLogger(&a);
    b.log(data);
    PrefixLogger<StderrLogger> c;
    c.log(data);
    c.log(data);
    StderrLogger* my_log = nullptr;
    SkatesRent<StderrLogger> sk(my_log);
    sk.addSkates("L", 5);
    sk.curSkates("L");
    return 0;
}