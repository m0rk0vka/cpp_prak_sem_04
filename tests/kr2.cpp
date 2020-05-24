q#include <iostream>
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

class SwitchLogger : public Logger {
    private:
        std::vector<Logger*> logs;
        Logger* cur;
        size_t cur_pos;
    public:
        SwitchLogger();
        void add(Logger *logger);
        virtual void log(const std::string & message) override;
        void sswitch();
};

SwitchLogger::SwitchLogger(){
    cur_pos = -1;
}

void SwitchLogger::add(Logger *logger) {
    logs.push_back(logger);
    if (cur_pos == -1){
        cur = logs[0];
        cur_pos = 0;
    }
}

void SwitchLogger::log(const std::string & message){
    if (cur_pos != -1){
        cur->log(message);
    } else {
        cout << "l" << endl;
        throw std::logic_error("current pointer undefined");
    }
}

void SwitchLogger::sswitch(){
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

class StderrLogger{
    public:
        void log(std::string message);
};

void StderrLogger::log(std::string message){
    std::cerr << message << std::endl;
}

template <class L>
class AlignLogger{
    private:
        L logger;
        int len_obrez;
    public:
        AlignLogger(int n = 20);
        void log(std::string message);
};

template<class L>
AlignLogger<L>::AlignLogger(int n){
    len_obrez = n;
}

template<class L>
void AlignLogger<L>::log(std::string message){
    std::string str = "";
    int i = 0;
    while (i < len_obrez && i < message.size()) {
        str += message[i];
        ++i;
    }
    while (i < len_obrez) {
        str += " ";
        ++i;
    }
    logger.log(str);
}

struct Rooms {
    std::string name;
    int maximum;
    int currently;
};

template <class T>
class MuseumRooms {
    private:
        std::vector<Rooms> rooms;
        T *logger;
    public:
        MuseumRooms(T *logger_init);
        void setRoom(std::string name, int maximum);
        void inRoom(std::string name);
        void outRoom(std::string name);
        int curInRoom(std::string name);
};

template<class T>
MuseumRooms<T>::MuseumRooms(T *logger_init) {
    logger = logger_init;
}


template<class T>
void MuseumRooms<T>::setRoom(std::string name, int maximum) {
    rooms.push_back(Rooms{name, maximum, 0});
    logger->log("Added room " + name);
}

template<class T>
void MuseumRooms<T>::inRoom(std::string name) {
    for (int i = 0 ; i < rooms.size(); i++) {
        if (rooms[i].name == name) {
            if (rooms[i].currently < rooms[i].maximum){
                ++rooms[i].currently;
                logger->log("Added 1 in " + name);
            } else {
                logger->log("Room " + name + " is fool");
            }
            return;
        }
    }
    logger->log("No such room");
}

template<class T>
void MuseumRooms<T>::outRoom(std::string name) {
    for (int i = 0 ; i < rooms.size(); i++) {
        if (rooms[i].name == name) {
            if (rooms[i].currently > 0){
                --rooms[i].currently;
                logger->log("1 people go out " + name);
            }
            return;
        }
    }
    logger->log("No such room");
}

template<class T>
int MuseumRooms<T>::curInRoom(std::string name){
    for (int i = 0 ; i < rooms.size(); i++) {
        if (rooms[i].name == name) {
            logger->log("Currently in " + name + " is " + std::to_string(rooms[i].currently) + " people");
            return rooms[i].currently;
        }
    }
    logger->log("Currently 0 in " + name);
    return 0;
}

int main(void)
{
    std::string data = "Q";
    FileLogger a("asd.txt");
    a.log(data);
    SwitchLogger b;
    b.add(&a);
    b.log(data);
    SwitchLogger d;
    try {
        d.log(data);
    } catch (const std::logic_error& e) {
        std::cerr << e.what() << std::endl;
    }
    AlignLogger<StderrLogger> c(10);
    c.log(data);
    c.log(data);
    StderrLogger* my_log = nullptr;
    MuseumRooms<StderrLogger> room(my_log);
    room.setRoom("A", 5);
    room.curInRoom("A");
    room.inRoom("A");
    room.inRoom("A");
    room.curInRoom("A");
    room.outRoom("A");
    room.curInRoom("A");
    return 0;
}