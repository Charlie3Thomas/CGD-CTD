#pragma once

#include "materials/material.hpp"

#include <embree3/rtcore.h>
#include <assimp/scene.h>

#include <unordered_map>


namespace CT
{
class EmbreeSingleton
{
public:
    static EmbreeSingleton& GetInstance();    
    RTCDevice device;
    RTCScene  scene;

    bool HasMaterial(unsigned int id) const;
    void AddMaterial(unsigned int id, const Material& material);
    Material& GetMaterial(unsigned int id);

    bool HasTexture(unsigned int id) const;
    void AddTexture(unsigned int id, const Texture& texture);
    Texture& GetTexture(unsigned int id);
    
    bool HasMesh(const std::string& name) const;
    void AddMesh(const std::string& name, aiMesh* mesh);
    aiMesh& GetMesh(const std::string& name);

    

private:
    EmbreeSingleton();

    EmbreeSingleton(const EmbreeSingleton&) = delete;

    EmbreeSingleton& operator=(const EmbreeSingleton&) = delete;

    EmbreeSingleton(EmbreeSingleton&&) = delete;

    EmbreeSingleton& operator=(EmbreeSingleton&&) = delete;

    ~EmbreeSingleton();

    std::unordered_map<std::string, aiMesh*> meshes;
    std::unordered_map<unsigned int, Material> materials;
    std::unordered_map<unsigned int, Texture> textures;   

};
}