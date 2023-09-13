#ifndef WEATHER_STATS_H
#define WEATHER_STATS_H

#include <string>

// Target Interface
class WeatherStats
{
public:
    std::string virtual GetTodayWeather() { return ""; };
};

#endif