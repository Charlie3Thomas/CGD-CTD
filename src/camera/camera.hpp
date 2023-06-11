#pragma once

#include <limits>

#include <Eigen/Core>
#include <Eigen/Dense>

#include <embree3/rtcore_ray.h>

namespace CT
{
class Camera
{
public:
    //TODO: There is the potential for dataloss as only the Z component of the look direction is used
    Camera(Eigen::Vector3f pos, Eigen::Vector3f lookdir, Eigen::Vector3f updir, float fl);


    /// @brief Fire a ray at a pixel on the camera canvas
    /// @param canvas_x 
    /// @param canvas_y 
    /// @return RTCRayHit (embree3/rtcore_ray.h)
    RTCRayHit GetRayForPixel(Eigen::Vector2i canvas_size, Eigen::Vector2i pixel_index);

    void HandlePixelData(const std::vector<float>& rgb);

private:
    // Camera position
    Eigen::Vector3f _pos;

    // Camera look direction
    Eigen::Vector3f _lookdir;

    // Camera up and right vectors
    Eigen::Vector3f _updir;
    Eigen::Vector3f _rightdir;

    // Camera field of view
    float _fl;

    // Camera clipping planes
    float _t_near = 0.001F;
    float _t_far  = std::numeric_limits<float>::infinity();    
};
}