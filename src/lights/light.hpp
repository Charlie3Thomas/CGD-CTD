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
                            const Eigen::Vector3f& incident_reflection, const Object* obj, const Lights& lights, RTCIntersectContext& context, size_t depth)
{
    RGB sample_light = BLACK;

    for (const auto& dir_light : lights.directional)
    {
        if (CastShadowRay(incident_hit_worldspace, dir_light.direction, std::numeric_limits<float>::infinity(), context))
            continue;
        // Calculate the diffuse component
        float costheta = std::max(0.0F, incident_shading_normal.dot(dir_light.direction)) / std::numbers::pi_v<float>;
        sample_light += (obj->material->kd * dir_light.colour * costheta);
        if (!obj->material->mirror) { continue; }
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
        if (!obj->material->mirror) { continue; }
        // Calculate the specular component
        float cosphi = std::max(0.0F, incident_reflection.dot(direction_to_point));
        sample_light += (obj->material->ks * point.colour * std::pow(cosphi, obj->material->shininess) * r2);
    }

    for (const auto& area_c : lights.area_cuboid)
    {  
        RGB bsdf = BLACK;
        Eigen::Vector3f area_light_sample_point;
        if (depth == 0)
        {
            float x = RandomRange(-0.5F, 0.5F);
            float z = RandomRange(-0.5F, 0.5F);

            Eigen::Vector3f rand_point_offset(x * area_c.width, 0.0F, z * area_c.height);
            area_light_sample_point = area_c.position + rand_point_offset;
        }
        else // Treat area light as a point light after the first bounce
        {
            area_light_sample_point = area_c.position;
        }

        // Calculate the direction to the random point on the area light
        Eigen::Vector3f direction_to_area_light = area_light_sample_point - incident_hit_worldspace;

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
        sample_light += (bsdf * area_c.colour * geomterm) / pdf;
    }

    return sample_light;
}

}