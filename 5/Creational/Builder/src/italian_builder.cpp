#include <string>

#include <iostream>

#include "italian_builder.h"
    
using namespace std;

void ItalianBuilder::SelectSize(std::string _size)
{
    size = _size;
}

void ItalianBuilder::SelectType(std::string _type)
{
    type = _type;
}

void ItalianBuilder::SelectSauce(std::string _sauce)
{
    sauce = _sauce;
}

void ItalianBuilder::SelectAdditive(std::string _additive)
{
    additive = _additive;
}

void ItalianBuilder::PresentPizza()
{
    cout << "PIZZA READY WITH SIZE of " << size << " And TYPE of " << type;
    cout << " Added with " << sauce << " Sauce and " << additive << endl;  
}