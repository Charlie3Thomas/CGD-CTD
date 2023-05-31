#pragma once

#include "camera.hpp"

namespace cameras
{
    
class digital : public camera
{
public:
    spacial_t position { 0, 0, 0 };
    double fov = 1;
    
    ray_t generate_ray(std::size_t hori, std::size_t vert) const override;

    void set_viewdir(const spacial_t& viewdir);
    void set_updir(const spacial_t& updir);

private:
    spacial_t _viewdir { 1,  0, 0 };
    spacial_t _updir   { 0,  0, 1 };
    spacial_t _leftdir { 0, -1, 0 };

private:
    void set_leftdir();
};

}