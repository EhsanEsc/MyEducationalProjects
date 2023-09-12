#ifndef ITALIAN_BUILDER_H
#define ITALIAN_BUILDER_H

#include <string>

#include "pizza_builder.h"

class ItalianBuilder : public PizzaBuilder
{
private:
    std::string size, type, sauce, additive;
public:
    void SelectSize(std::string _size) override;
    void SelectType(std::string _type) override;
    void SelectSauce(std::string _sauce) override;
    void SelectAdditive(std::string _additive) override;

    void PresentPizza();
};

#endif