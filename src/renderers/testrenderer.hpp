#pragma once

#include "renderer.hpp"

namespace CT
{
class TestRenderer : public Renderer
{
public:
    void InitialiseThreadPool();
    void RenderFilm(Film& film, Camera& camera, size_t threads) override;
};
}