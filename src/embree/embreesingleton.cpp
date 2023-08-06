#include "embreesingleton.hpp"
#include <cassert>


namespace CT
{
EmbreeSingleton& EmbreeSingleton::GetInstance()
{
    static EmbreeSingleton instance;
    return instance;
}

EmbreeSingleton::EmbreeSingleton() : device(rtcNewDevice(nullptr)), scene(rtcNewScene(device)) 
{
    assert(device != nullptr);
    assert(scene != nullptr);

    // Materials
    assert(!materials.contains("jade"));
    materials.emplace("jade",  std::make_unique<Mat>(JADE));
    assert(!materials.contains("copper"));
    materials.emplace("copper", std::make_unique<Mat>(COPPER));
    assert(!materials.contains("silver"));
    materials.emplace("silver", std::make_unique<Mat>(SILVER));
    assert(!materials.contains("mirror"));
    materials.emplace("mirror", std::make_unique<Mat>(SILVER));
    assert(!materials.contains("white_d"));
    materials.emplace("white_d", std::make_unique<Mat>(WHITE_D));
    assert(!materials.contains("white_s"));
    materials.emplace("white_s", std::make_unique<Mat>(WHITE_S));
    assert(!materials.contains("black_d"));
    materials.emplace("black_d", std::make_unique<Mat>(BLACK_D));
    assert(!materials.contains("black_s"));
    materials.emplace("black_s", std::make_unique<Mat>(BLACK_S));
    assert(!materials.contains("red_d"));
    materials.emplace("red_d", std::make_unique<Mat>(RED_D));
    assert(!materials.contains("green_d"));
    materials.emplace("green_d", std::make_unique<Mat>(GREEN_D));
    assert(!materials.contains("light"));
    materials.emplace("light", std::make_unique<Mat>(LIGHT));
}

EmbreeSingleton::~EmbreeSingleton()
{
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}
}