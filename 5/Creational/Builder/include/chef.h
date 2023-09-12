#ifndef CHEF_H
#define CHEF_H

#include "pizza_builder.h"

class Chef
{
public:
    Chef() {}
    
    void BakePizza(PizzaBuilder& builder);
};


#endif