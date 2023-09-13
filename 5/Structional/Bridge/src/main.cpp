#include <iostream>

#include "message_application.h"
#include "message_platforms.h"

using namespace std;

int main()
{
    SMSPlatform sms_platform;
    EmailPlatform email_platform;

    MessageApp app(sms_platform);
    app.SendMessage("Hello", 22);
    
    MessageApp app2(email_platform);
    app2.SendMessage("Bye", 322);

    return 0;
}