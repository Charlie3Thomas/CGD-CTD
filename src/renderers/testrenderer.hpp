#pragma once

#include "renderer.hpp"

#include <Eigen/Dense>
#include <embree3/rtcore.h>
#include "loaders/object.hpp"

namespace CT
{
class TestRenderer : public Renderer
{
public:
    void InitialiseThreadPool();
    void RenderFilm(Film& film, Camera& camera, size_t threads) override;
};
}