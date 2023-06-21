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
}

EmbreeSingleton::~EmbreeSingleton()
{
    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}
}