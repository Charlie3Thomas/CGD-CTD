#include "light.hpp"

namespace CT
{
class PointLight : public Light
{
public:
    PointLight(Eigen::Vector3f& position, const float intensity);

    // Sample the light and return it's intensity
    float SampleLight() const override;

    // Evaluate the light intensity at a point
    float EvaluateLight(const Eigen::Vector3f& point) const override;

    // Return the probability density function of the light
    float Pdf(const Eigen::Vector3f& point) const override;

private:
    Eigen::Vector3f position;
    float intensity;
    
};    
}