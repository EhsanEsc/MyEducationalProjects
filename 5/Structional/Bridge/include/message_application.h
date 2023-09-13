#ifndef MESSAGE_APPLICATION_H
#define MESSAGE_APPLICATION_H

#include <string>

#include "message_platforms.h"

class MessageApp
{
private:
    MessagePlatform& platform;
public:
    MessageApp(MessagePlatform& platform)
    : platform(platform)
    {}
    void SendMessage(std::string message, int user_id);
};


#endif