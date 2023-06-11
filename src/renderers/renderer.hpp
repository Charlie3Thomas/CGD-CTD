#pragma once

namespace CT
{
class Film;
class Camera;

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void RenderFilm(Film& film, Camera& camera) = 0;
};
}