#ifndef STATS_BUILDER_H
#define STATS_BUILDER_H

#include <string>

#include "pizza_builder.h"

class StatsBuilder : public PizzaBuilder
{
private:
    int small_count = 0;
public:
    void SelectSize(std::string size) override;

    void PresentStats();
};

#endif