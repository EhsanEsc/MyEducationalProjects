#ifndef CAR_H
#define CAR_H

#include <string>

class Car
{
protected:
    std::string name;
public:
    Car(std::string name)
    : name(name)
    {}
    void virtual Drive() = 0;
};

class Peraid : public Car
{
public:
    Peraid(std::string name)
    : Car(name)
    {}
    void virtual Drive() override;
};

#endif