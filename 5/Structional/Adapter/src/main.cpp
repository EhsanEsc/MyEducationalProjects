#include <iostream>

#include "weather_stats.h"
#include "third_party.h"

using namespace std;

void broadcast_weather(WeatherStats& weather)
{
    cout << "WEATHER IS " << weather.GetTodayWeather() << endl;
}

int main()
{
    AerisWeatherWrapper aeris;
    MeteomaticsWeatherWrapper meteomatics;

    broadcast_weather(aeris);
    broadcast_weather(meteomatics);

    return 0;
}

