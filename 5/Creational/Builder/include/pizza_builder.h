#ifndef PIZZA_BUILDER_H
#define PIZZA_BUILDER_H

#include <string>

class PizzaBuilder
{
public:
    void virtual SelectSize(std::string size) {}
    void virtual SelectType(std::string type) {}
    void virtual SelectSauce(std::string sauce) {}
    void virtual SelectAdditive(std::string additive) {}
};

#endif