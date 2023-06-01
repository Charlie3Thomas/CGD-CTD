#include <limits>

#include "ppm.hpp"

namespace CT
{
std::ostream& PPMWriteHeader(std::ostream& os, std::size_t width, std::size_t height)
{
    return os << "P3\n" << width << ' ' << height << '\n' << static_cast<int>(std::numeric_limits<std::uint8_t>::max()) << '\n';
}

std::ostream& PPMWritePixel(std::ostream& os, CT::RGB rgb)
{
    return os << rgb << '\n';
}
}

