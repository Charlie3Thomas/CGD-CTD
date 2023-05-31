#include "parallelepiped.h"

parallelepiped::parallelepiped(const spacial_t& x, const spacial_t& y, const spacial_t& z)
{
	_parallelograms[0] = parallelogram({ _centre, _centre + x, _centre + y });
	_parallelograms[1] = parallelogram({ _centre, _centre + y, _centre + z });
	_parallelograms[2] = parallelogram({ _centre, _centre + z, _centre + x });
	_parallelograms[3] = parallelogram({ _centre + x + y + z, _centre + y + z, _centre + x + z });
	_parallelograms[4] = parallelogram({ _centre + x + y + z, _centre + x + z, _centre + x + y });
	_parallelograms[5] = parallelogram({ _centre + x + y + z, _centre + x + y, _centre + y + z });
}

std::optional<object::hit_info> parallelepiped::get_hit_info(const ray_t& ray) const
{
	for (const auto& i : _parallelograms)
	{
		const std::optional<hit_info> colour = i.get_hit_info(ray);
		if (colour.has_value())
			return colour;
	}

	return std::optional<hit_info>();
}

void parallelepiped::set_centre(const spacial_t& centre)
{
	// Also reset centre for each of our parallelograms as well
	_centre = centre;
	for (auto& i : _parallelograms)
		i.set_centre(_centre);
}