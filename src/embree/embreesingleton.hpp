#pragma once

#include "materials/material.hpp"

#include <embree3/rtcore.h>

#include <unordered_map>


namespace CT
{
class EmbreeSingleton
{
public:
    RTCDevice device;
    RTCScene  scene;

    static EmbreeSingleton& GetInstance();

    // Maps
    std::unordered_map<unsigned int, Material> materials;

    void AddMaterial(unsigned int id, const Material& material);

private:
    EmbreeSingleton();

    EmbreeSingleton(const EmbreeSingleton&) = delete;

    EmbreeSingleton& operator=(const EmbreeSingleton&) = delete;

    EmbreeSingleton(EmbreeSingleton&&) = delete;

    EmbreeSingleton& operator=(EmbreeSingleton&&) = delete;

    ~EmbreeSingleton();
};
}