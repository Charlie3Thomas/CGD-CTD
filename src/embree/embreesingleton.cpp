#include "embreesingleton.hpp"
#include <cassert>


namespace CT
{
EmbreeSingleton& EmbreeSingleton::GetInstance()
{
    static EmbreeSingleton instance;
    return instance;
}

bool EmbreeSingleton::HasMaterial(unsigned int id) const
{
    return materials.find(id) != materials.end();
}

void EmbreeSingleton::AddMaterial(unsigned int id, const Material& material)
{
    assert(materials.find(id) == materials.end());
    materials[id] = material;
}

Material& EmbreeSingleton::GetMaterial(unsigned int id)
{
    assert(materials.find(id) != materials.end());
    return materials[id];
}

bool EmbreeSingleton::HasTexture(unsigned int id) const
{
    return textures.find(id) != textures.end();
}

void EmbreeSingleton::AddTexture(unsigned int id, const Texture& texture)
{
    assert(textures.find(id) == textures.end());
    textures[id] = texture;
}

Texture& EmbreeSingleton::GetTexture(unsigned int id)
{
    assert(textures.find(id) != textures.end());
    return textures[id];
}

bool EmbreeSingleton::HasMesh(const std::string& name) const
{
    return meshes.find(name) != meshes.end();
}

void EmbreeSingleton::AddMesh(const std::string& name, aiMesh* mesh)
{
    assert(meshes.find(name) == meshes.end());
    meshes[name] = mesh;
}

aiMesh& EmbreeSingleton::GetMesh(const std::string& name)
{
    assert(meshes.find(name) != meshes.end());
    return *meshes[name];
}

EmbreeSingleton::EmbreeSingleton() : device(rtcNewDevice(nullptr)), scene(rtcNewScene(device)) 
{
    assert(device != nullptr);
    assert(scene != nullptr);
}

EmbreeSingleton::~EmbreeSingleton()
{
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}
}