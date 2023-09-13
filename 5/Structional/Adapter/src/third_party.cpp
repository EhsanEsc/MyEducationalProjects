#include "third_party.h"

std::string AerisWeather::GetAerisWeather()
{
    return "Cloudy";
}

std::string AerisWeatherWrapper::GetTodayWeather()
{
    return GetAerisWeather();
}

std::string MeteomaticsWeather::GetMeteomaticsWeather(std::string date)
{
    if (date == "Today")
        return "Rainy";
    else
        return "Sunny";
}

std::string MeteomaticsWeatherWrapper::GetTodayWeather()
{
    return meteo.GetMeteomaticsWeather("Today");
}