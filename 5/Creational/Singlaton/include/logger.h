#ifndef LOGGER_H
#define LOGGER_H

#include <string>

class Logger
{
private:
    Logger() {}
public:
    static Logger& GetInstance();
    void error(std::string output);
    void info(std::string output);
};

#endif