#ifndef THIRD_PARTY_H
#define THIRD_PARTY_H

#include <string>

#include "weather_stats.h"

class AerisWeather
{
public:
    std::string GetAerisWeather();
};

class AerisWeatherWrapper: public WeatherStats, private AerisWeather
{
public:
    std::string GetTodayWeather() override;
};

class MeteomaticsWeather
{
public:
    std::string GetMeteomaticsWeather(std::string date);
};

class MeteomaticsWeatherWrapper : public WeatherStats
{
private:
    MeteomaticsWeather meteo;
public:
    std::string GetTodayWeather() override;
};

#endif