#include <iostream>
#include <memory>

#include "saipa_factory.h"

using namespace std;

int main()
{
    // You can set factory to something else to change functionality based on factory
    SaipaFactory factory;

    std::shared_ptr<Car> my_car = factory.create_car();
    std::shared_ptr<Motorcycle> my_motor = factory.create_motorcycle();

    my_car->Drive();
    my_motor->Wheelie();

    return 0;
}