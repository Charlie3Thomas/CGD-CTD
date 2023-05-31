#include "digital.hpp"

namespace cameras
{

ray_t digital::generate_ray(std::size_t hori, std::size_t vert) const
{
    if (hori >= f.horizontal_pixels || vert >= f.vertical_pixels)
        throw std::runtime_error("Invalid pixel positon");

    const auto aspect_ratio = static_cast<double>(f.horizontal_pixels)/static_cast<double>(f.vertical_pixels);
    const spacial_t view = aspect_ratio * (0.5 - static_cast<double>(hori + 1)/static_cast<double>(f.horizontal_pixels)) * _leftdir
                                        + (0.5 - static_cast<double>(vert + 1)/static_cast<double>(f.vertical_pixels))   * _updir;
    return { .origin = position, .direction = math::normalise(_viewdir + fov * view) };
}

void digital::set_viewdir(const spacial_t& viewdir)
{
    _viewdir = math::normalise(viewdir);

}

void digital::set_updir(const spacial_t& updir)
{
    _updir = math::normalise(updir);
}

void digital::set_leftdir()
{
    if (math::dot_product(_viewdir, _updir) > std::numeric_limits<double>::epsilon())
        throw std::runtime_error("Invalid camera orientation - viewdir and updir are not orthogonal");

    _leftdir = math::cross_product(_viewdir, _updir);
}

}