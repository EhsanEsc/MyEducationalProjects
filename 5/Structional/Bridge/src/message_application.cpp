#include <string>
#include <iostream>

#include "message_application.h"

using namespace std;

void MessageApp::SendMessage(std::string message, int user_id)
{
    platform.SendMessage(message, user_id);
}

