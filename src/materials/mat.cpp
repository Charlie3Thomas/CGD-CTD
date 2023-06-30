#include "materials/mat.hpp"
#include "utils/utils.hpp"

namespace CT
{
Vector3f Mat::SampleHemishphere(const Vector3f& normal)
{
    float u1 = RandomRange(0.0F, 1.0F);
    float u2 = RandomRange(0.0F, 1.0F);

    float radius = std::sqrt(u1);
    float theta = 2.0F * M_PIf32 * u2;

    float x = radius * std::cos(theta);
    float y = radius * std::sin(theta);

    return { x, y, std::sqrt(std::max(0.0F, 1.0F - u1)) };
}
}