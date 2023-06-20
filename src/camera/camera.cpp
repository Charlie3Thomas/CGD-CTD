#include "camera.hpp"
#include "film.hpp"

#include <cassert>
#include <numbers>


namespace CT
{

Camera::Camera(Eigen::Vector3f pos, Eigen::Vector3f lookdir, Eigen::Vector3f updir, float fl) 
    : _pos(pos), _lookdir(lookdir), _updir(updir), _rightdir(updir.cross(_lookdir)), _fl(fl) { }

RTCRayHit Camera::GetRayForPixel(const Canvas& canvas, Eigen::Vector2i pixel_index) const
{   
    // Return ray
    RTCRayHit ret;

    // Ray origin
    ret.ray.org_x = _pos.x();
    ret.ray.org_y = _pos.y();
    ret.ray.org_z = _pos.z();

    // Aspect ratio
    const auto& film = canvas.GetFilm();
    const float aspect_ratio = static_cast<float>(film.rect.GetWidth()) / static_cast<float>(film.rect.GetHeight());

    // Ray direction
    const size_t film_x = pixel_index.x() + canvas.rect.ulx;
    const size_t film_y = pixel_index.y() + canvas.rect.uly;
    Eigen::Vector3f ray_dir = _lookdir * _fl + 
        _rightdir * (0.5F - static_cast<float>(film_x) / static_cast<float>(film.rect.GetWidth())) * aspect_ratio +
        _updir    * (0.5F - static_cast<float>(film_y) / static_cast<float>(film.rect.GetHeight()));

    // Normalize ray direction
    ray_dir.normalize();

    // Ray direction
    ret.ray.dir_x = ray_dir.x();
    ret.ray.dir_y = ray_dir.y();
    ret.ray.dir_z = ray_dir.z();

    // Clipping planes
    ret.ray.tnear = _t_near;
    ret.ray.tfar  = _t_far;

    // Layer mask
    ret.ray.mask  = -1;

    return ret;
}
}