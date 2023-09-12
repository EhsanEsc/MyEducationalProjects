#ifndef SAIPA_FACTORY_H
#define SAIPA_FACTORY_H

#include <memory>

#include "vehicle_factory.h"
#include "car.h"
#include "motorcycle.h"

class SaipaFactory : public VehicleFactory
{
public:
    virtual std::shared_ptr<Motorcycle> create_motorcycle() override;
    virtual std::shared_ptr<Car> create_car() override;
};

#endif