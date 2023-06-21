#include "utils/rgb.hpp"

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

RGB FromTexture(RTCHit& hit, const Texture* tex)
{  
    // Calculate the texture coordinates scaled by texture dimensions
    float u = hit.u * static_cast<float>(tex->width);
    float v = hit.v * static_cast<float>(tex->height);

    int x = static_cast<int>(u);
    int y = static_cast<int>(v);

    int texel_offset = static_cast<int>((x + y * tex->width) * 3);

    BYTE b = tex->buffer[texel_offset];
    BYTE g = tex->buffer[texel_offset + 1];
    BYTE r = tex->buffer[texel_offset + 2];

    return 
    { 
        static_cast<float>(r) / 255.0F, 
        static_cast<float>(g) / 255.0F, 
        static_cast<float>(b) / 255.0F 
    };
    // // Get barycentric coordinates
    // const float u = hit.u;
    // const float v = hit.v;
    // const float w = 1.0F - u - v;    

    // Vector2f tca(tri.v[0].pos.x(), tri.v[0].pos.y());
    // Vector2f tcb(tri.v[1].pos.x(), tri.v[1].pos.y());
    // Vector2f tcc(tri.v[2].pos.x(), tri.v[2].pos.y());

    // Vector2f interp_tex_coords = u * tca + v * tcb + w * tcc;

    // int texel_offset = static_cast<int>(interp_tex_coords.x()) + static_cast<int>(interp_tex_coords.y());

    // assert(static_cast<size_t>(texel_offset) < tex.buffer.size());

    // BYTE blue  = tex.buffer[texel_offset];
    // BYTE green = tex.buffer[texel_offset + 1];
    // BYTE red   = tex.buffer[texel_offset + 2];

    // // Return RGB
    // return 
    // {
    //     static_cast<float>(red)   / 255.0F,
    //     static_cast<float>(green) / 255.0F,
    //     static_cast<float>(blue)  / 255.0F
    // };
}
}