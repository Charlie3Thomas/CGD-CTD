#include "utils/rgb.hpp"

#include <Eigen/Core>

#include <cassert>

using namespace Eigen;

namespace CT
{
RGB FromIntersectNormal(const RTCHit& hit)
{
    return 
    {
        std::abs(hit.Ng_x), 
        std::abs(hit.Ng_y), 
        std::abs(hit.Ng_z)
    };
}

RGB FromNormal(Vector3f flimbo)
{
    return 
    {
        std::abs(flimbo.x() / 2 + 0.5F),
        std::abs(flimbo.y() / 2 + 0.5F),
        std::abs(flimbo.z() / 2 + 0.5F)
    };
}

RGB FromBaryCoords(const RTCHit& hit)
{
    return 
    {
        hit.u,
        hit.v,
        1.0F - hit.u - hit.v
    };
}

RGB FromTexture(const RTCHit& hit, const Texture* tex, const UVTextureCoords& tex_coords)
{  
    float baru = hit.u;
    float barv = hit.v;
    float barw = 1.0F - hit.u - hit.v;

    float Au = tex_coords.coords[1].x();
    float Av = tex_coords.coords[1].y();

    float Bu = tex_coords.coords[2].x();
    float Bv = tex_coords.coords[2].y();

    float Cu = tex_coords.coords[0].x();
    float Cv = tex_coords.coords[0].y();

    float u = baru * Au + barv * Bu + barw * Cu;
    float v = baru * Av + barv * Bv + barw * Cv;

    auto px = static_cast<unsigned int>(u * static_cast<float>(tex->width));

    auto py = static_cast<unsigned int>(v * static_cast<float>(tex->height));

    unsigned int p_index = (py * tex->width + px) * 3;

    return 
    {
        static_cast<float>(tex->buffer[p_index + 2]) / 255.0F,
        static_cast<float>(tex->buffer[p_index + 1]) / 255.0F,
        static_cast<float>(tex->buffer[p_index + 0]) / 255.0F
    };
}
}