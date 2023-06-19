#include "object.hpp"
#include "utils/utils.hpp"

namespace CT
{
Object::Object(RTCGeometry& geom, Material& mat, Texture& tex)
    : geom(geom), material(mat), texture(tex), id(GetGUID()) {}

Object::~Object()
{
    rtcReleaseGeometry(geom);
    material.~Material();
    texture.~Texture();
}
}