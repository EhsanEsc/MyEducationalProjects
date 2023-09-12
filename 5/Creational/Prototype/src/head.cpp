#include <string>
#include <memory>

#include "head.h"

using namespace std;

std::shared_ptr<Head> Head::Clone()
{
    return make_shared<Head>();
}

string Head::Present()
{
    return "Normal HEAD";
}

std::shared_ptr<Head> TriangleHead::Clone()
{
    return make_shared<TriangleHead>();
}

string TriangleHead::Present()
{
    return "Triangle HEAD";
}
