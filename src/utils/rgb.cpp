#include "utils/rgb.hpp"

#include <Eigen/Core>

#include <cassert>

using namespace Eigen;

namespace CT
{
RGB FromIntersectNormal(RTCHit& hit)
{
    return 
    {
        hit.Ng_x * 100, 
        hit.Ng_y * 100, 
        hit.Ng_z * 100
    };
}

RGB FromBaryCoords(RTCHit& hit)
{
    return 
    {
        hit.u,
        hit.v,
        1.0F - hit.u - hit.v
    };
}

RGB FromTexture(RTCHit& hit, const Texture* tex, const UVTextureCoords& tex_coords)
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
    //assert(u >= 0.0F && u <= 1.0F);
    float v = baru * Av + barv * Bv + barw * Cv;
    //assert(v >= 0.0F && v <= 1.0F);
    float w = 1.0F - u - v;

    auto px = static_cast<unsigned int>(u * static_cast<float>(tex->width));
    //assert(px <= tex->width);
    auto py = static_cast<unsigned int>(v * static_cast<float>(tex->height));
    //assert(py <= tex->height);

    unsigned int p_index = (py * tex->width + px) * 3;
    //assert(p_index + 2 < tex->width * tex->height * 3);

    return 
    {
        static_cast<float>(tex->buffer[p_index + 2]) / 255.0F,
        static_cast<float>(tex->buffer[p_index + 1]) / 255.0F,
        static_cast<float>(tex->buffer[p_index + 0]) / 255.0F
        // u,
        // v,
        // w
    };
}
}