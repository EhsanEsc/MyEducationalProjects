#ifndef MESSAGE_PLATFORMS_H
#define MESSAGE_PLATFORMS_H

#include <string>

// Base Implementor Class
class MessagePlatform
{
public:
    void virtual SendMessage(std::string message, int user_id) = 0;
};

// Extended version of implementation
class SMSPlatform : public MessagePlatform
{
public:
    void SendMessage(std::string message, int user_id) override;
};

class EmailPlatform : public MessagePlatform
{
public:
    void SendMessage(std::string message, int user_id) override;
};

#endif