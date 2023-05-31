#include "polygon.h"

#include <sstream>

std::optional<object::hit_info> polygon::get_hit_info(const ray_t& ray) const
{
	// Make our bounding box if we haven't already
	if (!_bounding_box.has_value())
		make_bounding_box();
	const parallelepiped& bounding_box = _bounding_box.value();

	// Return false straight away if we don't hit the bounding box
	if (!bounding_box.get_hit_info(ray).has_value())
		return std::optional<hit_info>();

	std::optional<hit_info> ret;
	for (const auto& i : _triangles)
	{
		const auto h_i = i.get_hit_info(ray);
		if (h_i.has_value() && 
			(!ret.has_value() || 
				ret.value().z > h_i.value().z))
		{
			ret = h_i;
		}
	}

	return ret;
}

void polygon::set_centre(const spacial_t& centre)
{
	// Also reset centre for each of our triangles and parallelepiped as well
	_centre = centre;
	for (auto& i : _triangles)
		i.set_centre(_centre);

	_bounding_box.reset();
}

std::istream& operator>>(std::istream& is, polygon& v)
{
	std::vector<spacial_t> vertexes;
	std::vector<unstd::vector<size_t, 3>> mesh;

	// Read until the end of the file
	while (is)
	{
		// Get the prefix of the line.
		std::string prefix;
		is >> prefix;

		if (prefix == "v")
		{
			// Read the vertex positions
			vertexes.emplace_back();
			spacial_t& vertex = vertexes.back();

			is >> vertex[0] >> vertex[1] >> vertex[2];
		}
		else if (prefix == "f")
		{
			// Read the mesh (how all the vertexes are joined together to create triangles
			mesh.emplace_back();
			unstd::vector<size_t, 3>& net = mesh.back();

			// We only care about the first coordinate for now
			for (auto& i : net)
				is >> i;
		}

		// Read until the end of the line
		is.ignore(64, '\n');
	}

	// Create the triangle vector out of these
	v._triangles.clear();
	for (const auto& net : mesh)
	{
		v._triangles.emplace_back();
		auto& triangle = v._triangles.back();

		triangle.mat.colour = v.pgon_colour;
		for (size_t j = 0; j < net.size(); j++)
		{
			triangle[j] = vertexes[net[j] - 1];
		}
	}

	// Reset the centres of all the triangles
	v.set_centre(v._centre);

	return is;
}

std::ostream& operator<<(std::ostream& os, const polygon& v)
{
	return os;
}

void polygon::make_bounding_box() const
{
	timer t("make_bounding_box");
	// Find the minimum and maximum x / y / z coordinates
	spacial_t min = _triangles[0][0];
	spacial_t max = _triangles[0][0];
	for (const auto& triangle : _triangles)
		for (const auto& vertex : triangle)
			for (size_t i = 0; i < vertex.size(); i++)
			{
				min[i] = std::min(min[i], vertex[i]);
				max[i] = std::max(max[i], vertex[i]);
			}

	// Make the diagonal vector that spans the whole polygon
	const spacial_t diagonal = max - min;

	// Make the x, y, z coordinates for the parallelepiped
	const spacial_t x = { diagonal[0], 0, 0 };
	const spacial_t y = { 0, diagonal[1], 0 };
	const spacial_t z = { 0, 0, diagonal[2] };

	// Make the parallelepiped with the centre set properly
	_bounding_box = parallelepiped(x, y, z);
	_bounding_box.value().set_centre(_centre + min);
}