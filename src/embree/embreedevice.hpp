#pragma once

#include <embree3/rtcore.h>

class EmbreeSingleton
{
public:
    RTCDevice device;
    RTCScene scene;

    static EmbreeSingleton& GetInstance();

private:
    EmbreeSingleton();

    EmbreeSingleton(const EmbreeSingleton&) = delete;

    EmbreeSingleton& operator=(const EmbreeSingleton&) = delete;

    EmbreeSingleton(EmbreeSingleton&&) = delete;

    EmbreeSingleton& operator=(EmbreeSingleton&&) = delete;

    ~EmbreeSingleton();
};