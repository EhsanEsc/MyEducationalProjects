#include "message_platforms.h"

#include <iostream>
#include <string>

using namespace std;

void SMSPlatform::SendMessage(std::string message, int user_id)
{
    cout << "SMS \"" << message << "\" sent to " << user_id << endl;
}

void EmailPlatform::SendMessage(std::string message, int user_id)
{
    cout << "Email \"" << message << "\" sent to " << user_id << endl;
}
