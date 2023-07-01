#pragma once

#include "materials/mat.hpp"

#include <cmath>

namespace CT
{
    RGB Evaluate(const Mat* mat, const Eigen::Vector3f& normal, const Eigen::Vector3f& light_dir, 
                 const Eigen::Vector3f& incident_reflection, float l_intensity, float a_intensity, float attenuation)
    {
        float costheta = std::max(0.0F, normal.dot(light_dir));
        float cosphi   = std::max(0.0F, incident_reflection.dot(light_dir));

        const RGB I_ambient  = mat->ka * a_intensity;
        const RGB I_diffuse  = mat->kd * l_intensity * costheta;
        const RGB I_specular = mat->ks * attenuation * l_intensity * (std::pow(cosphi, mat->shininess));

        return I_diffuse + I_ambient + I_specular;
    }

    // Diffuse reflection
    /*
        float I_l      : Light source intensity
        float k_d      : Surface reflectance coefficient
        Vector3f theta : Light/Normal vector
    */

    // Ambient light
    /*
        float I_da : Ambient light intensity
        float I_a  : Ambient light intensity
        float k_a  : Ambient reflectance (local)
    */

   // Specular reflection
   /*
        float I_s  : = k_s * I * cos(phi)^n(shininess)
        float phi  : Angle between reflected light ray and -raydirection
        float k_s  : Specular reflectance coefficient
        float n    : Shininess (rate of specular fall-off)
   */

    // Putting it all together
    /*
        // f_att may be 1.0F for directional light
        // f_att should be a product of the inverse square law for point lights
        float I = (k_a * I_a) + (f_att * I_l * (k_d * cos(theta) + k_s * cos(phi)^n)
    */
}