#pragma once

#include "loaders/object.hpp"
#include "embree/embreesingleton.hpp"
#include "utils/rgb.hpp"
#include "utils/utils.hpp"

#include <Eigen/Dense>

#include <array>
#include <numbers>

namespace CT
{
struct DirectionalLight
{
    RGB colour;
    Eigen::Vector3f direction;
};

struct PointLight
{
    RGB colour;
    Eigen::Vector3f position;
};

struct AreaLightCuboid
{
    RGB colour;
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    float width;
    float height;
};

struct AreaLightSphere
{
    RGB colour;
    Eigen::Vector3f position;
    float radius;
};

struct ShadowRayInfo
{
    const Eigen::Vector3f& ray_hit_ws;
    const Eigen::Vector3f& ray_reflection_dir;
    RTCIntersectContext& context;
    const RTCScene& scene;
};

struct Lights
{
public:
    std::vector<DirectionalLight> directional;
    std::vector<PointLight> point;
    std::vector<AreaLightCuboid> area_cuboid;
    std::vector<AreaLightSphere> area_sphere;
};

static bool CastShadowRay(const Eigen::Vector3f& ray_hit_ws, const Eigen::Vector3f& light_dir_ws, float distance_to_light, RTCIntersectContext& context)
{
    RTCRay ray {
        .org_x = ray_hit_ws.x(),
        .org_y = ray_hit_ws.y(),
        .org_z = ray_hit_ws.z(),
        .tnear = 0.0001F,
        .dir_x = light_dir_ws.x(),
        .dir_y = light_dir_ws.y(),
        .dir_z = light_dir_ws.z(),
        .tfar  = distance_to_light,
        .mask  = 0xFFFFFFFF };
    
    rtcOccluded1(EmbreeSingleton::GetInstance().scene, &context, &ray);
    return ray.tfar < distance_to_light; // if tfar is less than distance to light, then there is an occluder
}

static RGB EvaluateLighting(const Eigen::Vector3f& incident_hit_worldspace, const Eigen::Vector3f& incident_shading_normal, 
                            const Eigen::Vector3f& incident_reflection, const Object* obj, const Lights& lights, RTCIntersectContext& context)
{
    RGB sample_light = BLACK;

    for (const auto& dir_light : lights.directional)
    {
        if (CastShadowRay(incident_hit_worldspace, dir_light.direction, std::numeric_limits<float>::infinity(), context))
            continue;
        // Calculate the diffuse component
        float costheta = std::max(0.0F, incident_shading_normal.dot(dir_light.direction)) / std::numbers::pi_v<float>;
        sample_light += (obj->material->kd * dir_light.colour * costheta);
        if (!obj->material->specular) { continue; }
        // Calculate the specular component
        float cosphi = std::max(0.0F, incident_reflection.dot(dir_light.direction));
        sample_light += (obj->material->ks * dir_light.colour * std::pow(cosphi, obj->material->shininess));
    }

    for (const auto& point : lights.point)
    {
        Eigen::Vector3f direction_to_point = point.position - incident_hit_worldspace;
        float distance_to_light = direction_to_point.norm();
        direction_to_point /= distance_to_light;
        if (CastShadowRay(incident_hit_worldspace, direction_to_point, distance_to_light, context))
            continue;
        float r2 = 1.0F / (distance_to_light * distance_to_light);
        // Calculate the diffuse component
        float costheta = std::max(0.0F, incident_shading_normal.dot(direction_to_point)) / std::numbers::pi_v<float>;
        sample_light += (obj->material->kd * point.colour * costheta * r2);
        if (!obj->material->specular) { continue; }
        // Calculate the specular component
        float cosphi = std::max(0.0F, incident_reflection.dot(direction_to_point));
        sample_light += (obj->material->ks * point.colour * std::pow(cosphi, obj->material->shininess) * r2);
    }

    for (const auto& area_c : lights.area_cuboid)
    {  
        RGB bsdf = BLACK;

        //float Kd = Luminance(obj->material->kd);
        //float Ks = Luminance(obj->material->ks);
        //float n = obj->material->shininess;
        //bool specular = obj->material->specular;

        ///// Randomly select whether to compute a specular or diffuse sample
        //float u1 = RandomRange(0.0F, 1.0F);
        //float u2 = RandomRange(0.0F, 1.0F);

        // Importance sampling of the Phong Reflectance Model
        /*
        Fr(x, theta iprime, theta o) = Fr,d(x, theta iprime, theta o) + Fr,s(x, theta iprime, theta o) = Kd * 1/PI + Ks * (n + 2)/(2*PI) * cos^n(a)
        where A = the angle between the perfect specular reflection direction and the direction of the incident light
            values greater than pi/2 are clamped to pi/2 to prevent negative values of the cosine factor
        Kd = the diffuse reflectivity (the fraction of the incoming energy that is reflected diffuseley)
        Ks = the specular reflectivity (the fraction of th perpendicularly incoming energy that is reflected specularly)
        n = the shininess (the exponent of the cosine factor)
        In order for the Phong brdf to have the property of energy conservation, the sum of the diffuse and specular reflectivities must be less than or equal to 1

        At the start of each iteration of the recursive step in our path tracer, we have the following known quantities:
            X = the point of intersection with the current ray and the scene
            N = the vector normal to the differential surface at the point of intersection (X)
            Wo = a unit vector in the firection of the current outgoing ray of light 
            Fr(x, theta iprime, theta o) = the Phong reflectance brdf described above with diffuse / speular parameters given by the material of the intersected surface

        Randomly select whether to compute a diffuse sample or specular sample.
            Generate a uniform random variable U in the range [0, 1], evoking the rule:

                u < Kd: take a diffuse sample and compute its contribution
                Kd <= u < Kd + Ks: take a specular sample and compute its contribution
                Kd + Ks <= u: the contribution of the current ray is zero

        PDF(theta prime) = 1/PI * cos(theta prime)

        Therefore, to generate a mapping from two uniform random variables u1, u2 in the range [0, 1] that follow this distribution, 
        we compute the separable marginal cumulative distribution functions of the PDF. We then take their inverses and obtain the following transformation:
            Wi = (theta, phi) = (arccos(sqrt(u1)), 2*PI*u2) (in spherical coordinates)

        For a specular sample we sish to generate random paths that favour the maximum contribution of energy as prescribed by the specular component of the Phong brdf.
        This demands a sampling that favours the region of maximum specular reflection 
            (i.e.: the "specular lobe" that exists along the direction of perfect specular reflective direciton w.r.t. the outgoing ray of light)
        This gives the following desired distribution:
            pdf(theta prime) = (n + 1)/(2*PI) * cos^n(alpha)
            where alpha is the angle between W (the incoming path of light) and the direction of perfect speular reflection w.r.t Wo (the outgoing path of light)

        In order to generate a mapping from two uniform random variables U1 and U2 in the range [0, 1] to a ray that follows this distribution,
        we compute the inverse of the cumulative distribution function, obtaining the following transformatino (in spherical coordinates)
            Wi = (alpha, phi) = (arccos(U1^(1/(n+1)) 2*PI*U2)
        */


        Eigen::Vector3f x = Eigen::Vector3f::Random();
        x.y() = 1.0F;
        Eigen::Vector3f y = Eigen::Vector3f::Random();
        y.y() = 1.0F;

        // Get random point on area light in worldspace
        Eigen::Vector3f random_point_on_area_light = area_c.position + Eigen::Vector3f::Random() * area_c.width + Eigen::Vector3f::Random() * area_c.height;
        //Eigen::Vector3f random_point_offset(x * area_c.width, y * area_c.height, 0.0f);
        //Eigen::Vector3f random_point_on_area_light = area_c.position + random_point_offset;
        // TODO : Not random Vector3f, two Random floats in range [0, 1]

        // Calculate the direction to the random point on the area light
        Eigen::Vector3f direction_to_area_light = random_point_on_area_light - incident_hit_worldspace;

        // Calculate the distance between the random point on the area light and the incident hit point
        float distance_to_area_light = direction_to_area_light.norm();

        // Normalise the direction to the area light
        direction_to_area_light /= distance_to_area_light;

        // If light is occluded, continue
        if (CastShadowRay(incident_hit_worldspace, direction_to_area_light, distance_to_area_light, context))
            continue;

        // Calculate PDF
        float pdf = 1.0F / (area_c.width * area_c.height);

        // Calculate attenuation
        float r2 = 1.0F / (distance_to_area_light * distance_to_area_light);

        float costheta = std::max(0.0F, incident_shading_normal.dot(direction_to_area_light));
        // TODO : Dot produce between the negative direction to the area light and the normal of the area light, add to multipliers
        float costhetaprime = std::max(0.0F, -area_c.normal.dot(direction_to_area_light));
        float geomterm = costheta * costhetaprime * r2;
        float cosphi = std::max(0.0F, incident_reflection.dot(direction_to_area_light));
        bsdf = (obj->material->kd / std::numbers::pi_v<float>) + (obj->material->ks * ((obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, obj->material->shininess)) * r2;
        //sample_light += (obj->material->kd * area_c.colour * costheta * r2) / pdf;
        sample_light += (bsdf * area_c.colour * geomterm) / pdf;
    }

    return sample_light;
}

}