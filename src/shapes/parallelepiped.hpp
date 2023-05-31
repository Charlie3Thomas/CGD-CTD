#pragma once

#include "parallelogram.h"

class parallelepiped: public shape
{
public:
	parallelepiped(const spacial_t& x, const spacial_t& y, const spacial_t& z);

	std::optional<hit_info> get_hit_info(const ray_t& ray) const override;

	void set_centre(const spacial_t& centre) override;

private:
	typedef std::array<parallelogram, 6> parallelograms_t;
	parallelograms_t _parallelograms;
};