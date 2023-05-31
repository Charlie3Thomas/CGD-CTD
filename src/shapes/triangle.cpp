#include "triangle.h"

triangle::triangle(const vertices_t& vertices)
	: _vertices(vertices) {}

std::optional<object::hit_info> triangle::get_hit_info(const ray_t& ray) const
{
	// The absolute position of the triangle
	const vertices_t absolute = { _vertices[0] + _centre, _vertices[1] + _centre, _vertices[2] + _centre };

	// Compute the normal of the plane the triangle is in (the cross product of two of its sides)
	const spacial_t side0 = absolute[0] - absolute[2];
	const spacial_t side1 = absolute[1] - absolute[0];
	const spacial_t side2 = absolute[2] - absolute[1];

	// Calculate the normal if necessary
	if (!_normal.has_value()) { make_normal(); }
		
	const spacial_t& normal = _normal.value();

	// Compute how aligned the normal to the triangle and the direction of the ray are (their dot product)
	const floating_point_t alignment = -unstd::dot_product(normal, ray.direction);

	// We do not count the ray as hitting if direction is almost orthoganal to the normal of the triangle, or if it
	// is hitting the backwards face of the triangle
	if (alignment < EPSILON) return std::optional<colour_t>();

	// Compute the distance along the ray which intersects with the plane
	const floating_point_t t = unstd::dot_product(normal, ray.origin - absolute[0]) / alignment;

	// If t is negative the triangle is behind the camera
	if (t < 0) return std::optional<colour_t>();

	// Compute the actual point of interesection and the displacement vector to this point
	const spacial_t intersection = ray[t];

	// Are we right of the first side?
	if (unstd::scalar_triple_product(normal, side0, intersection - absolute[2]) < 0) return std::optional<colour_t>();

	// Are we right of the second side?
	if (unstd::scalar_triple_product(normal, side1, intersection - absolute[0]) < 0) return std::optional<colour_t>();

	// Are we right of the third side?
	if (unstd::scalar_triple_product(normal, side2, intersection - absolute[1]) < 0) return std::optional<colour_t>();
	
	/*
		r = d -2(d . n)n
		where 
		r = incoming ray vector
		2 because incoming anble x 2
		d . n = alignment
		n = normal vector of surface hit
	*/

	spacial_t reflection = ray.direction + (2 * alignment * normal);
	reflection.normalise();

	return hit_info(mat.colour * alignment, (intersection - ray.origin).length(), ray_t(intersection, reflection));
}

triangle::vertex_t& triangle::operator[](size_t i)
{ 
	// We have to assume the vertices will be changed
	_normal.reset();
	return _vertices[i]; 
}

const triangle::vertex_t& triangle::operator[](size_t i) const
{ 
	return _vertices[i]; 
}

void triangle::make_normal() const
{
	_normal = unstd::cross_product(_vertices[0] - _vertices[2], _vertices[1] - _vertices[0]);
	_normal.value().normalise();
}