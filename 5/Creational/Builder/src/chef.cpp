#include "chef.h"

void Chef::BakePizza(PizzaBuilder& builder)
{
    builder.SelectSize("Small");
    builder.SelectType("Peperoni");
    builder.SelectSauce("Spicy");
    builder.SelectAdditive("Mushroom");
}