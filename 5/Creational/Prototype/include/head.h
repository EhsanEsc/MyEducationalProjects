#ifndef HEAD_H
#define HEAD_H

#include <string>

class Head
{
public:
    std::shared_ptr<Head> virtual Clone();
    std::string virtual Present();
};

class TriangleHead : public Head
{
public:
    std::shared_ptr<Head> Clone() override;
    std::string Present() override;
};

#endif