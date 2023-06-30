#pragma once

#include <Eigen/Dense>
#include <array>

#include "utils/rgb.hpp"
#include "utils/utils.hpp"

using namespace Eigen;

namespace CT
{

struct MaterialProperties
{
    Vector3f base_colour;
    float    metalness;
    Vector3f emissive;
    float    roughness;
    float    transmissivness;
    float    opacity;
};

struct BRDFData
{
    // Material properties
    Vector3f specularF0;
    Vector3f diffuse_reflectance;

    // Roughness
    float roughness;
    float alpha;
    float alpha_squared;

    // Fresnel
    Vector3f F;

    // Vectors
    Vector3f V; // Direction to viewer (opposite of incident ray)
    Vector3f N; // Surface normal
    Vector3f H; // Half vector
    Vector3f L; // Direction to light source (or direction of reflecting ray)

    float NdotL;
    float NdotV;

    float LdotH;
    float NdotH;
    float VdotH;

    // True when V/L is backfacing
    bool V_backfacing;
    bool L_backfacing;
};


// Utility functions

/// @brief Converts shininess to roughness
/// @param shininess 
/// @return 
float ShininessToRoughness(float shininess)
{
    return std::sqrt(2.0F / (shininess + 2.0F));
}

float AlphaToShininess(float alpha)
{
    return 2.0F / std::min(0.9999F, std::max(0.0002F, (alpha * alpha))) - 2.0F;
}

/// @brief Converts roughness to shininess
/// @param roughness 
/// @return 
float RoughnessToShininess(float roughness)
{
    return 2.0F / (roughness * roughness) - 2.0F;
}

/// @brief Returns the luminance of an RGB colour
/// @param rgb 
/// @return 
float Luminance(const Vector3f& rgb)
{
    return rgb.dot(Vector3f(0.2126F, 0.7152F, 0.0722F));
}

/// @brief Reflects an incident vector about a normal
/// @param incident 
/// @param normal 
/// @return 
Vector3f Reflect(const Vector3f& incident, const Vector3f& normal)
{
    return incident - 2.0F * incident.dot(normal) * normal;
}

/// @brief Rotates a point by a quaternion
/// @param q 
/// @param v 
/// @return 
Vector3f RotatePoint(const Quaternionf& q, Vector3f v)
{
    Quaternionf p(0, v.x(), v.y(), v.z());

    return (q * p * q.inverse()).vec();
}

Quaternionf GetRotationFromZAxis(Vector3f input)
{
    // Handle case where input is exact or near opposite of (0, 0, 1)
    if (input.z() < -0.99999F) { return Quaternionf{ 1, 0, 0, 0 }; }

    Vector3f q_axis = Vector3f(-input.y(), input.x(), 0.0F).normalized();
    float q_angle = 2.0F * std::acos(std::sqrt(input.z() + 1.0F));

    return Quaternionf(AngleAxisf(q_angle, q_axis));
}



// Sampling methods

/// @brief Samples a cosine-weighted hemisphere
/// @param u Randomly generated vector2f
/// @param pdf 
/// @return 
Vector3f SampleHemisphere(Vector2f u, float& pdf)
{
    float a = std::sqrt(u.x());
    auto  b = static_cast<float>((2.0F * M_PI) * u.y());

    Vector3f ret = Vector3f
    (
        a * std::cos(b),
        a * std::sin(b),
        std::sqrt(1.0F - u.x())
    );
    
    pdf = static_cast<float>(ret.z() * (1.0F / (1.0F / M_PI)));

    return ret;
}

Vector3f EvaluateFresnelSchlick(const Vector3f& F0, float f90, float NdotS)
{
    return F0 + Vector3f(f90 - F0.x(), f90 - F0.y(), f90 - F0.z()) * std::pow(1.0F - NdotS, 5.0F);
}

float ShadowedF90(const Vector3f& F0)
{
    const float t = 1.0F / 0.04F;
    return std::min(1.0F, t * Luminance(F0));
}


// -------------------------------------------------------------------------
//    Lambert
// -------------------------------------------------------------------------
float Lambertian(const BRDFData& data)
{
    return 1.0F;
}

Vector3f EvalLambertian(const BRDFData& data)
{
    return data.diffuse_reflectance * ((1.0F / M_PI) * data.NdotL);
}

// -------------------------------------------------------------------------
//    Phong
// -------------------------------------------------------------------------
float PhongNormalisationTerm(float shininess)
{
    return static_cast<float>((1.0F + shininess) * (1.0F / (2.0F * M_PI)));
}

Vector3f EvalPhong(const BRDFData& data)
{
    // Convert roughness to shininess (phong exponent)
    float shininess = AlphaToShininess(data.alpha);

    Vector3f R = Reflect(data.L, data.N);
    return data.specularF0 * (PhongNormalisationTerm(shininess) * std::pow(std::max(0.0F, R.dot(data.V)), shininess) * data.NdotL);
}

Vector3f SamplePhong(/*Vector3f v_local, */float shininess, Vector2f u, float& pdf)
{
    float cos_theta = std::pow(1.0F - u.x(), 1.0F / (1.0F + shininess));
    float sin_theta = std::sqrt(1.0F - cos_theta * cos_theta);

    auto phi = static_cast<float>((2.0F * M_PI) * u.y());

    pdf = PhongNormalisationTerm(shininess) * std::pow(cos_theta, shininess);

    return Vector3f
    {
        std::cos(phi) * sin_theta,
        std::sin(phi) * sin_theta,
        cos_theta
    };
}

Vector3f SampleSpecularPhong(const Vector3f& v_local, float alpha, float alpha_squared, const Vector3f& specularF0, Vector2f u, Vector3f& weight)
{
    float shininess = AlphaToShininess(alpha);

    float pdf = 0.0F;
    Vector3f lobe_phong = SamplePhong(shininess, std::move(u), pdf);

    // lobe_phong is in "lobe space" - it is centred around the +Z axis
    // It must be rotated in direction of perfect reflection
    Vector3f n_local = Vector3f(0.0F, 0.0F, 1.0F);
    Vector3f lobe_direction = Reflect(-v_local, n_local);
    Vector3f l_local = RotatePoint(GetRotationFromZAxis(lobe_direction), lobe_phong);

    // Calculate weight of the sample
    Vector3f r_local = Reflect(-v_local, n_local);
    float NdotL = std::max(0.00001F, n_local.dot(l_local));
    weight = specularF0 * (PhongNormalisationTerm(shininess) * std::pow(std::max(0.0F, (r_local.dot(v_local))), shininess) * NdotL) / pdf;

    return l_local;
}

float Saturate(float x)
{
    return std::clamp(x, 0.0F, 1.0F);
    //return std::min(std::max(x, 0.0F), 1.0F);
}

Vector3f BaseColourToSpecularF0(const Vector3f& base_colour, float metalness)
{
    return Vector3f(0.04F, 0.04F, 0.04F) + (base_colour - Vector3f(0.04F, 0.04F, 0.04F)) * metalness;
}

Vector3f BaseColourToDiffuseReflectance(const Vector3f& base_colour, float metalness)
{
	return base_colour * (1.0F - metalness);
}

/// @brief Sets up commonly used data for BRDF evaluation
/// @param N Surface normal
/// @param L Direction of the reflecting ray
/// @param V Direction to viewer (camera)
/// @param material 
/// @return 
BRDFData PreProcessBRDFData(const Vector3f& N, const Vector3f& L, const Vector3f& V, const MaterialProperties& material)
{
    BRDFData data;

    data.V = V;
    data.N = N;
    data.H = (L + V).normalized();
    data.L = L;

    float NdotL = N.dot(L);
    float NdotV = N.dot(V);
    data.V_backfacing = NdotV < 0.0F;
    data.L_backfacing = NdotL < 0.0F;

    // Clamp NdotS to prevent bad things, assuming vectors below the hemisphere are filtered with V and L backfacing bools
    data.NdotL = std::clamp(NdotL, 0.00001F, 1.0F);
    data.NdotV = std::clamp(NdotV, 0.00001F, 1.0F);

    data.LdotH = Saturate(L.dot(data.H));
    data.NdotH = Saturate(N.dot(data.H));
    data.VdotH = Saturate(V.dot(data.H));

    // Unpack material properties
    data.specularF0 = BaseColourToSpecularF0(material.base_colour, material.metalness);
    data.diffuse_reflectance = BaseColourToDiffuseReflectance(material.base_colour, material.metalness);

    data.roughness = material.roughness;
    data.alpha = material.roughness * material.roughness;
    data.alpha_squared = data.alpha * data.alpha;

    data.F = EvaluateFresnelSchlick(data.specularF0, ShadowedF90(data.specularF0), data.VdotH);

    return data;
}

/// @brief Evaluate the specular and diffuse components of the BRDF
/// @param N Surface normal
/// @param L Direction of reflecting ray
/// @param V Direction to viewer (camera) - opposite of incident ray
/// @param material The material used to evaluate the BRDF
/// @return Returns a colour value
RGB EvalCombined(const Vector3f& N, const Vector3f& L, const Vector3f& V, const MaterialProperties& material)
{
    // Prepare data
    const BRDFData data = PreProcessBRDFData(N, L, V, material);

    // Ignore V and L rays below the hemisphere
    if (data.V_backfacing || data.L_backfacing) { return RGB { 0.0F, 0.0F, 0.0F }; }

    // Evaluate specular and diffuse
    Vector3f specular = EvalPhong(data);
    Vector3f diffuse  = EvalLambertian(data);

    // Combine specular and diffuse layers
#if 1
    Vector3f ret = (Vector3f(1.0F, 1.0F, 1.0F).array() - data.F.array()) * diffuse.array() + data.F.array() * specular.array();
    return RGB { ret.x(), ret.y(), ret.z() };
     
#else
    Vector3f ret = diffuse + specular;
    //Vector3f ret = specular;
    return RGB { ret.x(), ret.y(), ret.z() };
#endif
}


bool EvalIndirectCombinedBDRF(Vector2f u, Vector3f shading_normal, Vector3f geom_normal, Vector3f V, MaterialProperties material, Vector3f& ray_dir, Vector3f& sample_weight)
{
    // Ignore incident ray coming from below the hemisphere
    if (shading_normal.dot(V) < 0.0F)
    { 
        return false;
    }

    // Transform view direction into local space of sampling
    Quaternionf q_rotate_to_z = GetRotationFromZAxis(shading_normal);
    Vector3f V_local = RotatePoint(q_rotate_to_z, V);
    const Vector3f N_local = Vector3f(0.0F, 0.0F, 1.0F);

    Vector3f ray_direction_local = Vector3f { 0.0F, 0.0F, 0.0F };

    float pdf = 0.0F;

    // Sample diffuse ray using cosine-weighted hemisphere sampling
    Vector2f u_diffuse = Vector2f(RandomRange(0.0F, 1.0F), RandomRange(0.0F, 1.0F));
    ray_direction_local = SampleHemisphere(u_diffuse, pdf);
    const BRDFData data = PreProcessBRDFData(N_local, ray_direction_local, V_local, material);
    
    Vector3f h_specular = SamplePhong(material.metalness, u, pdf);

    float VdotH = std::max(0.00001F, std::min(1.0F, V_local.dot(h_specular)));
    sample_weight.array() *= (Vector3f(1.0F, 1.0F, 1.0F).array() - EvaluateFresnelSchlick(data.specularF0, ShadowedF90(data.specularF0), VdotH).array());

    // Prevent tracing direction with no contribution
    if (Luminance(sample_weight) == 0.0F) 
    { 
        return false; 
    }

    // Transform sampled direction L_local back into V vector space
    ray_dir = (RotatePoint(q_rotate_to_z.conjugate(), ray_direction_local)).normalized();

    // Prevent tracing directino under the hemisphere
    if (geom_normal.dot(ray_dir) < 0.0F) 
    { 
        //return false;
    }

    return true;
}

}