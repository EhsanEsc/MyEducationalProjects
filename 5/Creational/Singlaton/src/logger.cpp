#include <iostream>

#include "logger.h"

using namespace std;

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::error(string output)
{
    cerr << "Error: " << output << endl;
}

void Logger::info(string output)
{
    cout << "Info: " << output << endl;
}