#include <iostream>

#include "stats_builder.h"

using namespace std;

void StatsBuilder::SelectSize(std::string size)
{
    small_count += (size == "Small");
}

void StatsBuilder::PresentStats()
{
    cout << "Number of small pizzas: " << small_count << endl;
}