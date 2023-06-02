#pragma once

#include <random>
#include <array>

#include "rgb.hpp"

namespace CT
{

RGB RandomColour()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 6);

    std::array<RGB, 8> colours = {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, WHITE};

    return colours[dis(gen)];
}
}