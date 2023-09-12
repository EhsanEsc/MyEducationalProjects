#ifndef VEHICLE_FACTORY_H
#define VEHICLE_FACTORY_H

#include <memory>

#include "car.h"
#include "motorcycle.h"

class VehicleFactory
{
public:
    virtual std::shared_ptr<Motorcycle> create_motorcycle() = 0;
    virtual std::shared_ptr<Car> create_car() = 0;
};

#endif