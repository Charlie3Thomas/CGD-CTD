#include "camera.hpp"


#include <cassert>
#include <numbers>

using namespace CT;

Camera::Camera(Eigen::Vector3f pos, Eigen::Vector3f lookdir, Eigen::Vector3f updir, float fl) 
    : _pos(pos), _lookdir(lookdir), _updir(updir), _rightdir(updir.cross(_lookdir)), _fl(fl) {}

RTCRayHit Camera::GetRayForPixel(Eigen::Vector2i canvas_size, Eigen::Vector2i pixel_index)
{   
    // Return ray
    RTCRayHit ret;

    // Ray origin
    ret.ray.org_x = _pos.x();
    ret.ray.org_y = _pos.y();
    ret.ray.org_z = _pos.z();

    // Aspect ratio
    const float aspect_ratio = static_cast<float>(canvas_size.x()) / static_cast<float>(canvas_size.y());

    // Ray direction
    Eigen::Vector3f ray_dir = _lookdir * _fl + 
        _rightdir * (0.5F - static_cast<float>(pixel_index.x()) / static_cast<float>(canvas_size.x())) * aspect_ratio +
        _updir    * (0.5F - static_cast<float>(pixel_index.y()) / static_cast<float>(canvas_size.y()));

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