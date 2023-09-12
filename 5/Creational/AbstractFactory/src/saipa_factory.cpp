#include <iostream>

#include "saipa_factory.h"

using namespace std;

std::shared_ptr<Car> SaipaFactory::create_car()
{
    return make_shared<Peraid>("Peraid Meshki");
}

std::shared_ptr<Motorcycle> SaipaFactory::create_motorcycle()
{
    return make_shared<Changan>("Changan Sefid");
}