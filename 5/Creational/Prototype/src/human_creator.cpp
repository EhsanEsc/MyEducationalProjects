#include <iostream>
#include <memory>

#include "human_creator.h"
#include "head.h"

using namespace std;

void HumanCreator::PresentNewHuman()
{
    shared_ptr<Head> head = head_prototype->Clone();
    cout << "My head is " << head->Present() << endl;
}