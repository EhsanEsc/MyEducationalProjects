#include <memory>

#include "logger.h"

using namespace std;

int main()
{
    Logger& logger = Logger::GetInstance();

    logger.info("Hello");
    logger.error("Bye");

    return 0;
}