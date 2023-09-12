using namespace std;

#include <memory>

#include "human_creator.h"
#include "head.h"

int main()
{
    HumanCreator creator1(make_shared<Head>());
    creator1.PresentNewHuman();
    
    HumanCreator creator2(make_shared<TriangleHead>());
    creator2.PresentNewHuman();

    return 0;
}