#pragma once

#include "materials/mat.hpp"
#include "textures/texture.hpp"

#include <embree3/rtcore.h>
#include <assimp/scene.h>

#include <unordered_map>
#include <memory>


namespace CT
{
class EmbreeSingleton
{
public:
    static EmbreeSingleton& GetInstance();
    RTCDevice device;
    RTCScene  scene;
    
    std::unordered_map<std::string, std::unique_ptr<Mat>>     materials;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

private:
    EmbreeSingleton();

    EmbreeSingleton(const EmbreeSingleton&) = delete;

    EmbreeSingleton& operator=(const EmbreeSingleton&) = delete;

    EmbreeSingleton(EmbreeSingleton&&) = delete;

    EmbreeSingleton& operator=(EmbreeSingleton&&) = delete;

    ~EmbreeSingleton();
};
}