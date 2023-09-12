#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include <string>

class Motorcycle
{
protected:
    std::string name;
public:
    Motorcycle(std::string name)
    : name(name)
    {}
    void virtual Wheelie() = 0;
};

class Changan : public Motorcycle
{
public:
    Changan(std::string name)
    : Motorcycle(name)
    {}
    void virtual Wheelie() override;
};

#endif