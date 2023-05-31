#pragma once

#include "shape.hpp"

namespace shapes
{

/**
 * @brief The unit disk
 * Satisfies equation x^2 + y^2 = 1, z = 0
 */
class disk : public shape
{
public:
    virtual ~disk() = default;

    std::unique_ptr<shape> clone() const override { return std::make_unique<disk>(*this); }

    bool hit(const ray_t& ray) const override;
    bool hit(const ray_t& ray, intersection& intersec) const override;

    boundary get_boundary() const override;
};

}