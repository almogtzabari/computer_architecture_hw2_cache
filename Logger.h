
#ifndef COMPUTER_ARCHITECTURE_HW2_CACHE_LOGGER_H
#define COMPUTER_ARCHITECTURE_HW2_CACHE_LOGGER_H

#include <iostream>

#define _LOG_LEVEL USER

using std::string;
using std::cout;
using std::endl;

enum LOG_LEVEL {
    USER = 0,
    TRACE = 1,
    DEBUG = 2
};


class LOG {
public:
    LOG() {}
    LOG(LOG_LEVEL logLevel) {
        msglevel = logLevel;
        operator << ("[" + LogLevelToString(logLevel) + "] ");
    }
    ~LOG() {
        if(opened) {
            cout << endl;
        }
        opened = false;
    }
    template<class T>
    LOG& operator<<(const T &msg) {
        if(msglevel <= _LOG_LEVEL) {
            cout << msg;
            opened = true;
        }
        return *this;
    }

private:
    bool opened = false;
    LOG_LEVEL msglevel = DEBUG;
    static const string LogLevelToString(LOG_LEVEL logLevel) {
        switch(logLevel) {
            case USER: return "USER";
            case TRACE: return "TRACE";
            case DEBUG: return "DEBUG";
            default: return "LOG_INVALID";
        }
    }
};

#endif  /* COMPUTER_ARCHITECTURE_HW2_CACHE_LOGGER_H */