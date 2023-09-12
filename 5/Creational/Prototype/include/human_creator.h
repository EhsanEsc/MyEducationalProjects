#ifndef HUMAN_CREATOR_H
#define HUMAN_CREATOR_H

#include "head.h"

class HumanCreator
{
private:
    std::shared_ptr<Head> head_prototype;
public:
    HumanCreator(std::shared_ptr<Head> head_prototype)
    : head_prototype(head_prototype)
    {}

    void PresentNewHuman();
};

#endif