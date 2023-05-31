#pragma once

#include "shape.hpp"

namespace shapes
{

/**
 * @brief The unit tube
 * Satisfies equation x^2 + y^2 = 1, -1 < z < 1
 */
class tube : public shape
{
public:
    virtual ~tube() = default;

    std::unique_ptr<shape> clone() const override { return std::make_unique<tube>(*this); }

    bool hit(const ray_t& ray) const override;
    bool hit(const ray_t& ray, intersection& intersec) const override;

    boundary get_boundary() const override;
};

}