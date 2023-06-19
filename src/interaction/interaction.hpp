#pragma once

#include <Eigen/Dense>
#include <embree3/rtcore.h>

using namespace Eigen;

struct Interaction
{
// Public methods
    //Interaction() : time(0) {}
    Interaction(const Vector3f &p, const Vector3f &n, const Vector3f &pError, const Vector3f &wo, float time)
        : p(p), n(n), pError(pError), wo(wo), time(time) {}


// Public data
    Vector3f p;
    Vector3f n;
    Vector3f pError;
    Vector3f wo;
    float time;
};