#include "chef.h"
#include "italian_builder.h"
#include "stats_builder.h"

using namespace std;

int main()
{
    // Concrete Builder
    ItalianBuilder italian_builder;
    StatsBuilder stats_builder;
    // Director
    Chef chef;
    
    chef.BakePizza(italian_builder);
    chef.BakePizza(stats_builder);
    chef.BakePizza(stats_builder);

    // Presentation could be different from other builders
    italian_builder.PresentPizza();
    stats_builder.PresentStats();

    return 0;
}