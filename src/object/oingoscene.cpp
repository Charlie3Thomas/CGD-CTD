#include "oingoscene.hpp"

#include <cassert>

namespace CT
{

Object& OingoScene::GetObjectOfID(uint64_t id)
{
    auto it = objects.find(id);

    assert(it != objects.end());

    return it->second;
}

void OingoScene::AddObject(Object& obj)
{
    assert(objects.find(obj.id) == objects.end());
    objects.insert(std::make_pair(obj.id, obj));
}
}