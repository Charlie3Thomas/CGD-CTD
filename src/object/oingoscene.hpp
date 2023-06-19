#pragma once

#include "object.hpp"

#include <unordered_map>

namespace CT
{
class OingoScene
{
public:
    std::unordered_map<uint64_t, Object> objects = std::unordered_map<uint64_t, Object>();
    Object& GetObjectOfID(uint64_t id);
    void AddObject(Object& obj);
};
}